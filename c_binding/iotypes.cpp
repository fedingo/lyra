#include <iostream>
#include <string>
#include <vector>
#include "iotypes.h"


extern "C" {
    #define ARRAY_TEMPLATE(type, struct_name) \
    /*Type Agnostic IO that convert from and to C++ std::vectors and C array Structs */\
    struct struct_name{\
        type* array;\
        int length;\
    };\
    struct_name* from_vector_##type (std::vector< type > data)\
    {\
        struct_name* output = (struct_name*)malloc(sizeof(struct_name));\
        output->length = data.size();\
        output->array = &data[0];\
        return output;\
    }\
    std::vector< type > to_vector_##type (const struct_name* data)\
    {\
        std::vector<type> output(data->array, data->array + data->length);\
        return output;\
    }\

    ARRAY_TEMPLATE(uint8_t, array_uint8_t)
    ARRAY_TEMPLATE(int16_t, array_int16_t)

    // Exported method to free memory after extracting the result from the pointer structure
    void Free(void *p)
    {
        free(p);
    }
}