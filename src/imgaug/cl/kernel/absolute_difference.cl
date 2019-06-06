#define saturate_8u(value) ( (value) > 255 ? 255 : ((value) < 0 ? 0 : (value) )
#define abs(value) ( (value) < 0 ? (-1 * value)  : value )
__kernel void absolute_difference(  __global unsigned char* a,
                                    __global unsigned char* b,
                                    __global unsigned char* c,
                                    const unsigned int height,
                                    const unsigned int width,
                                    const unsigned int channel
)
{
    int id_x = get_global_id(0);
    int id_y = get_global_id(1);
    int id_z = get_global_id(2);
    if (id_x >= width || id_y >= height || id_z >= channel) return;

    int pixIdx = id_x + id_y * width + id_z * width * height;

    int res = a[pixIdx] + b[pixIdx];
    res = abs(res);
    c[pixIdx] = saturate_8u(res);
}
