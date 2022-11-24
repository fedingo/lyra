#include <vector>
#include <string>


extern "C"  //Tells the compiler to use C-linkage for the next scope.
{
    /*Template definition based on type of array Struct and functions*/
    #define ARRAY_TEMPLATE_HEADER(type, struct_name)\
    /* Struct Definition */\
    typedef struct struct_name struct_name;\
    /* Convertion from std::vector */\
    struct_name *from_vector_##type(std::vector< type > data);\
    /* Convertion to a std::vector*/\
    std::vector< type > to_vector_##type(const struct_name *data);


    ARRAY_TEMPLATE_HEADER(uint8_t, array_uint8_t)
    ARRAY_TEMPLATE_HEADER(int16_t, array_int16_t)

    // Exported method to free memory after extracting the result from the pointer structure
    void Free(void *p);
}