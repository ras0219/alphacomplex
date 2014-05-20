//accept a buffer from provider.
//TO-DO: make it more generic. Currently we have logic being provider and gfx being reciever
//stores upto 2 pointers. if both are in use, oldest gets deleted, and newest gets pushed to front
template <class data_type>
void triple_buffer_send(data_type* bfr);


//give a reciever a buffer.
//it will pop it from the above storage. if nothing new, return NULL
template <class data_type>
data_type* triple_buffer_recieve();