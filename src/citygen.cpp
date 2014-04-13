#include "citygen.hpp"
#include "city.hpp"
#include "components/room.hpp"
#include "entities/citizen.hpp"
#include "entities/workroom.hpp"
#include "entities/entity.hpp"
#include "entities/filestorage.hpp"
#include "hydroponics/hydroponics.hpp"
#include "hydroponics/hydroponics_table.hpp"



enum RoomTypes
{
  Regular,
  FoodSupply,
  FileStorage,
  Infirmary,
  Engineering,
  RoomTypesCount
};

struct RoomProperties
{
  int height, width, seed, top, left, bottom, right;
  RoomTypes roomType;
  bool isCompound, floorOnly;
  bool doorN, doorE, doorW, doorS;
  RoomProperties(int iTop, int iLeft, int iHeight, int iWidth)
  {
    top = iTop;
    left = iLeft;
    bottom = iTop + iHeight - 1;
    right = iLeft + iWidth - 1;
    height = iHeight;
    width = iWidth;

    // randomly determine the room type
    roomType = (RoomTypes)(rand() % RoomTypes::RoomTypesCount);

    // 2/3 chance that the room has smaller rooms inside it
    isCompound = ((rand() % 3) != 0);

    // is the room purely floor
    floorOnly = false;
    if ((height < 3) || (width < 3))
    {
      floorOnly = true;
    }

    // assume you can put doors on all sides
    doorN = true;
    doorS = true;
    doorE = true;
    doorW = true;

    // if the wall size is too small then don't put
    // the door on that wall
    if (height < 3)
    {
      doorE = false;
      doorW = false;
    }

    // if the wall size is too small then don't put
    // the door on that wall
    if (width < 3)
    {
      doorN = false;
      doorS = false;
    }
  }
};

class MathHelper
{
public:
  static int GetFactor(int p)
  {
    int factor = (p / 2);
    for (; factor > 2; factor--)
    {
      if ((p % factor) == 0)
        return factor;
    }
    return p;
  }
};

#define CITY_GENERATION_DEPTH 2
#define ROOM_MIN_SEGMENTS 2
#define ROOM_MAX_SEGMENTS 6
CityProperties::CityProperties() {}
CityProperties::CityProperties(int iHeight, int iWidth) : height(iHeight), width(iWidth),
							  seed((int)time(0)), top(0),
							  left(0), bottom(height-1),
							  right(width-1)
{}

CityProperties::CityProperties(int iHeight, int iWidth, int iSeed) :
  height(iHeight), width(iWidth),
  seed(iSeed), top(0),
  left(0), bottom(height-1),
  right(width-1)
{}

struct CityGenerator {
  CityGenerator(City& city, CityProperties const& props) : city(city), props(props) {
    // set seed for the city generator
    srand(props.seed);

    // assume the entire city area is just ground
    for (int i = 0; i < city.getYSize(); ++i)
    {
      for (int j = 0; j < city.getXSize(); ++j)
      {
        city.tile(j, i) = { Tile::TileKind::ground };
      }
    }

    // recursively generate city
    // TODO: may be we can move this to initialize()
    generate(CITY_GENERATION_DEPTH);
  }

  // generates random int value bound by min and max
  // TODO: move to a helper file
  int randNext() { return rand(); }
  int randNext(int max)
  {
    if (max == 0) return 0;
    return rand() % max;
  }
  int randNext(int min, int max)
  {
    if (min >= max) return min;
    return min + (rand() % (max - min));
  }

  // Recursive city generator. Accepts 'depth' to determine 
  // the segmentation granularity.
  void generate(int depth)
  {
    // determine the number of segments to cut vertically and horizontally
    int vRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);
    int hRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);

    // determine the size of each segment
    int vSize = city.getYSize() / vRooms;
    int hSize = city.getXSize() / hRooms;

    // often the factorization might not be clean
    // so determine where the vertical oversized segment should go.
    int *vSegments = new int[vRooms];
    for (int i = 0; i < vRooms; i++)
      vSegments[i] = vSize;
    int vIndex = randNext(0, vRooms);
    vSegments[vIndex] = vSize + city.getYSize() - (vSize * vRooms);

    // often the factorization might not be clean
    // so determine where the horizontal oversized segment should go.
    int *hSegments = new int[hRooms];
    for (int i = 0; i < hRooms; i++)
      hSegments[i] = hSize;
    int hIndex = randNext(0, hRooms);
    hSegments[hIndex] = hSize + city.getXSize() - (hSize * hRooms);

    // Start placing rooms recursively
    for (int i = 0, top = 0; i < vRooms; i++)
    {
      for (int j = 0, left = 0; j < hRooms; j++)
      {
        // determine the coordinates of the room
        int height = vSegments[i], width = hSegments[j];
        int actualTop = top, actualLeft = left;

        // this is to avoid double walling each room
        // if removed each room will sit inside a previous bound
        // hence adding a wall for each level of depth
        if (i != 0)
        {
          actualTop = top - 1;
          height += 1;
        }
        if (j != 0)
        {
          actualLeft = left - 1;
          width += 1;
        }

        // create room properties, this determines what kind of room, if it has doors
        // where the doors are, orientation, etc
        RoomProperties rp(actualTop, actualLeft, height, width);

        // determine if the room is a boundry for smaller rooms
        if (rp.isCompound)
        {
          // build more rooms inside the boundary
          generate(rp, depth - 1);
        } else
        {
          // construct the room using the room properties
          build_room(rp);
        }

        left += hSegments[j];
      }

      top += vSegments[i];
    }

    // construct the perimeter wall
    build_perimeter_wall();

    // add RED entities at a random position
    add_entities(Security::Mask::RED, 2, 4);

    // add ORANGE entities at a random position
    add_entities(Security::Mask::ORANGE, 1, 1);

    // change doors to ground
    finalize_doors();

    // find room locations
    // TODO: this can be removed( see comment on method) 
    find_rooms();

    delete[] vSegments;
    delete[] hSegments;
  }

  // Recursive city generator. Accepts 'depth' to determine 
  // the segmentation granularity. This version uses room properties to 
  // perform segmentation.
  void generate(const RoomProperties& roomProperties, int depth)
  {
    // if the depth is reached draw floor 
    if (depth <= 0)
    {
      build_floor(roomProperties);
      return;
    }

    // determine the number of segments to cut vertically and horizontally
    int vRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);
    int hRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);

    // determine the size of each segment
    int vSize = roomProperties.height / vRooms;
    int hSize = roomProperties.width / hRooms;

    // if the size is too small return
    if ((vSize <= 0) || (hSize <= 0))
    {
      return;
    }

    // often the factorization might not be clean
    // so determine where the vertical oversized segment should go.
    int *vSegments = new int[vRooms];
    for (int i = 0; i < vRooms; i++)
      vSegments[i] = vSize;
    int vIndex = randNext(0, vRooms);
    vSegments[vIndex] = vSize + roomProperties.height - (vSize * vRooms);

    // often the factorization might not be clean
    // so determine where the horizontal oversized segment should go.
    int *hSegments = new int[hRooms];
    for (int i = 0; i < hRooms; i++)
      hSegments[i] = hSize;
    int hIndex = randNext(0, hRooms);
    hSegments[hIndex] = hSize + roomProperties.width - (hSize * hRooms);

    // Start placing rooms recursively
    for (int i = 0, top = roomProperties.top; i < vRooms; i++)
    {
      for (int j = 0, left = roomProperties.left; j < hRooms; j++)
      {
        // determine the coordinates of the room
        int height = vSegments[i], width = hSegments[j];
        int actualTop = top, actualLeft = left;

        // this is to avoid double walling each room
        // if removed each room will sit inside a previous bound
        // hence adding a wall for each level of depth
        if (i != 0)
        {
          actualTop = top - 1;
          height += 1;
        }
        if (j != 0)
        {
          actualLeft = left - 1;
          width += 1;
        }

        // create room properties, this determines what kind of room, if it has doors
        // where the doors are, orientation, etc
        RoomProperties rp(actualTop, actualLeft, height, width);

        // determine if the room is a boundry for smaller rooms
        if (rp.isCompound)
        {
          // build more rooms inside the boundary
          generate(rp, depth - 1);
        } else
        {
          // construct the room using the room properties
          build_room(rp);
        }

        left += hSegments[j];
      }
      top += vSegments[i];
    }

    delete[] vSegments;
    delete[] hSegments;
  }

  // adds a random number of entities of a specified type
  void add_entities(Security::Mask entity, int min, int max)
  {
    int count = randNext(min, max);
    for (int i = 0; i<count;)
    {
      int r = randNext(0, city.getYSize());
      int c = randNext(0, city.getXSize());

      if (city.tile(c, r).type == Tile::TileKind::ground)
      {
        city.ent(c, r).insert(new_citizen({ &city, c, r }, entity));
        i++;
      }
    }
  }

  // construct a room
  void build_room(const RoomProperties& rp)
  {
    // if the room is just a floor build it
    if (rp.floorOnly)
    {
      build_floor(rp);
      return;
    }

    // construct a specific room.
    switch (rp.roomType)
    {
    case RoomTypes::Regular:
      build_regular(rp);
      build_doors(rp);
      break;
    case RoomTypes::FileStorage:
      build_file_storage(rp);
      build_doors(rp);
      break;
    case RoomTypes::Engineering:
      build_engineering(rp);
      build_doors(rp);
      break;
    case RoomTypes::FoodSupply:
      build_food_supply(rp);
      build_doors(rp);
      break;
    case RoomTypes::Infirmary:
      build_infirmary(rp);
      build_doors(rp);
      break;
    default:
      break;
    }
  }
  void build_regular(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if (city.tile(j, i).type != Tile::TileKind::door)
        {
          if ((i == rp.top) || (i == rp.bottom) ||
              (j == rp.left) || (j == rp.right))
          {
            city.tile(j, i).type = Tile::TileKind::wall;
          } else
          {
            city.tile(j, i).type = Tile::TileKind::ground;
          }
        }
      }
    }
  }

  void build_file_storage(const RoomProperties& rp)
  {
    build_regular(rp);

    if ((rp.width >= 5) || (rp.height >= 5))
    {
      int top = rp.top + 2;
      int bottom = rp.bottom - 2;
      int left = rp.left + 2;
      int right = rp.right - 2;

      if (rp.height > rp.width)
      {
        int factor = MathHelper::GetFactor(rp.height - 4);
        if ((rp.height - 4) <= 8)
        {
          factor = rp.height;
        }

        for (int i = top; i <= bottom; i++)
        {
          for (int j = left; j <= right; j += 2)
          {
            if ((city.tile(j, i).type == Tile::TileKind::ground) && ((i % factor) != 0))
            {
              make_filingcabinet({ &city, j, i });
            }
          }
        }
      } else
      {
        int factor = MathHelper::GetFactor(rp.width - 4);
        if ((rp.width - 4) <= 8)
        {
          factor = rp.width;
        }

        for (int i = top; i <= bottom; i += 2)
        {
          for (int j = left; j <= right; j++)
          {
            if ((city.tile(j, i).type == Tile::TileKind::ground) && ((j % factor) != 0))
            {
              make_filingcabinet({ &city, j, i });
            }
          }
        }
      }

      make_filestorage({ &city, rp.left + 1, rp.top + 1, rp.width - 2, rp.height - 2 });
    }
  }

  void build_engineering(const RoomProperties& rp)
  {
    build_regular(rp);

    if (((rp.width >= 5) && (rp.height >= 8)) ||
        ((rp.width >= 8) && (rp.height >= 5)))
    {
      int top = rp.top + 2;
      int bottom = rp.bottom - 2;
      int left = rp.left + 2;
      int right = rp.right - 2;

      if (rp.height > rp.width)
      {
        if ((rp.height % 2) == 0)
        {
          for (int i = top; i <= (top + 1); i++)
          {
            for (int j = left; j <= right; j++)
            {
              if (city.tile(j, i).type == Tile::TileKind::ground)
              {
                city.tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        } else
        {
          for (int i = (bottom - 1); i <= bottom; i++)
          {
            for (int j = left; j <= right; j++)
            {
              if (city.tile(j, i).type == Tile::TileKind::ground)
              {
                city.tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        }
      } else
      {
        if ((rp.width % 2) == 0)
        {
          for (int i = top; i <= bottom; i++)
          {
            for (int j = left; j <= (left + 1); j++)
            {
              if (city.tile(j, i).type == Tile::TileKind::ground)
              {
                city.tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        } else
        {
          for (int i = top; i <= bottom; i++)
          {
            for (int j = (right - 1); j <= right; j++)
            {
              if (city.tile(j, i).type == Tile::TileKind::ground)
              {
                city.tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        }
      }
    }
  }

  void build_food_supply(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if (city.tile(j, i).type != Tile::TileKind::door)
        {
          if ((i == rp.top) || (i == rp.bottom) ||
              (j == rp.left) || (j == rp.right))
          {
            city.tile(j, i).type = Tile::TileKind::wall;
          } else
          {
              //city.tile(j, i).type = Tile::TileKind::foodsupply;
            if (rand() % 3 == 0)
              make_hydroponics_table({ &city, j, i });
          }
        }
      }
    }

    make_hydroponics_room({ &city, rp.left + 1, rp.top + 1, rp.width - 2, rp.height - 2 });
  }

  void build_infirmary(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if (city.tile(j, i).type != Tile::TileKind::door)
        {
          if ((i == rp.top) || (i == rp.bottom) ||
              (j == rp.left) || (j == rp.right))
          {
            city.tile(j, i).type = Tile::TileKind::wall;
          } else
          {
            city.tile(j, i).type = Tile::TileKind::infirmary;
          }
        }
      }
    }
  }

  // mark anything that is not already a wall or door as floor
  void build_floor(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if ((city.tile(j, i).type != Tile::TileKind::door) &&
            (city.tile(j, i).type != Tile::TileKind::wall))
        {
          city.tile(j, i).type = Tile::TileKind::ground;
        }
      }
    }
  }

  // Determines where the door has to be placed
  void build_doors(const RoomProperties& rp)
  {
    if (rp.doorN)
    {
      int i = (rp.width % 2 == 0) ? rp.width / 2 : rp.width / 2 + 1;
      city.tile(rp.left + i, rp.top).type = Tile::TileKind::door;
    }

    if (rp.doorS)
    {
      int i = (rp.width % 2 == 0) ? rp.width / 2 : rp.width / 2 + 1;
      city.tile(rp.left + i, rp.bottom).type = Tile::TileKind::door;
    }

    if (rp.doorE)
    {
      int i = (rp.height % 2 == 0) ? rp.height / 2 : rp.height / 2 + 1;
      city.tile(rp.right, rp.top + i).type = Tile::TileKind::door;
    }
    if (rp.doorW)
    {
      int i = (rp.height % 2 == 0) ? rp.height / 2 : rp.height / 2 + 1;
      city.tile(rp.left, rp.top + i).type = Tile::TileKind::door;
    }
  }

  // Generates a perimeter wall for the entire city
  // TODO: this can be optimized to speed up city generation
  void build_perimeter_wall()
  {
    for (int i = props.top; i < props.height; i++)
    {
      for (int j = props.left; j < props.width; j++)
      {
        if ((i == 0) || (i == props.bottom) ||
            (j == 0) || (j == props.right))
        {
          city.tile(j, i).type = Tile::TileKind::wall;
        }
      }
    }
  }

  // While generating rooms the doors are represented by 
  // TileKind::door this method replaces them with TileKind::ground
  void finalize_doors()
  {
    for (int j = props.top; j < props.height; j++)
    {
      for (int i = props.left; i < props.width; i++)
      {

        if (city.tile(i, j).type == Tile::TileKind::door)
        {
          city.tile(i, j).type = Tile::TileKind::ground;
        }
      }
    }
  }

  // Finds the rooms in the generated city
  // TODO: this can be removed since rooms are
  // procedurally generated we don't have to find them
  void find_rooms()
  {
    for (int j = props.top; j < props.height; j++)
    {
      for (int i = props.left; i < props.width; i++)
      {
        char ch = city.tile(i, j).type;
        if ((ch == Tile::TileKind::wall) ||
            (ch == Tile::TileKind::ground))
            continue;

        int kx, ky, w, h;
        for (kx = i; kx < city.getXSize(); ++kx) {
          if (city.tile(kx, j).type != ch)
            break;
        }
        for (ky = j; ky < city.getYSize(); ++ky) {
          for (int x = i; x < kx; ++x) {
            if (city.tile(x, ky).type != ch)
              goto finish_loop;
          }
        }
      finish_loop:
        w = kx - i;
        h = ky - j;

        for (int y = j; y < ky; ++y) {
          for (int x = i; x < kx; ++x) {
            city.tile(x, y).type = Tile::ground;
          }
        }

        // room's top-left is i,j and dimensions are w x h
        city.rooms.push_back(make_workroom({ &city, i, j, w, h })->assert_get<Room>());
      }
    }
  }

  City& city;
  CityProperties const& props;

  CityGenerator& operator=(CityGenerator const&) = delete;
};

void randomgen(City& city, CityProperties const& props) {
  CityGenerator cg(city, props);
}
