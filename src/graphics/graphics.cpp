#include "graphics.hpp"

const Graphics::Context Graphics::colors_to_context[DEFAULT + 1] =
{
	{ 255, 255, 255, 255 },       //white
	{ 255, 0, 0, 255 },      //red
	{ 0, 255, 0, 255 },	   //green
	{ 0, 0, 255, 255 },      //blue
	{ 255, 255, 0, 255 },    //yellow
	{ 150, 75, 0, 255 },     //brown according to wiki
	{ 255, 165, 0, 255 },    //orange according to wiki
	{ 0, 0, 0, 0 },          //DEFAULT

};
