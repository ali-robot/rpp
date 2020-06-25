
#include <cl/rpp_cl_common.hpp>
#include "cl_declarations.hpp"

/************* Fish eye ******************/
RppStatus
fisheye_cl(cl_mem srcPtr, RppiSize srcSize,
           cl_mem dstPtr,
           RppiChnFormat chnFormat, unsigned int channel,
           rpp::Handle &handle)
{
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
        handle.AddKernel("", "", "fish_eye.cl", "fisheye_planar", vld, vgd, "")(srcPtr,
                                                                                dstPtr,
                                                                                srcSize.height,
                                                                                srcSize.width,
                                                                                channel);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
        handle.AddKernel("", "", "fish_eye.cl", "fisheye_packed", vld, vgd, "")(srcPtr,
                                                                                dstPtr,
                                                                                srcSize.height,
                                                                                srcSize.width,
                                                                                channel);
    }
    return RPP_SUCCESS;
}

RppStatus
fisheye_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
                 RppiChnFormat chnFormat, unsigned int channel)

{
    int plnpkdind;

    if(chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = 3;

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.csrcSize.height, handle.GetInitHandle()->mem.mgpu.csrcSize.width, handle.GetBatchSize(), &max_height, &max_width);

    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
    handle.AddKernel("", "", "fish_eye.cl", "fisheye_batch", vld, vgd, "")(srcPtr,
                                                                            dstPtr,
                                                                            handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                            handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                            handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                            handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                            handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                            handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                            handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,
                                                                            handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                            channel,
                                                                            handle.GetInitHandle()->mem.mgpu.inc,
                                                                            plnpkdind);
    return RPP_SUCCESS;
}
/************* LensCorrection ******************/
RppStatus
lens_correction_cl(cl_mem srcPtr, RppiSize srcSize, cl_mem dstPtr,
                   float strength, float zoom,
                   RppiChnFormat chnFormat, unsigned int channel,
                   rpp::Handle &handle)
{
    if (strength == 0)
        strength = 0.000001;
    float halfWidth = (float)srcSize.width / 2.0;
    float halfHeight = (float)srcSize.height / 2.0;
    float correctionRadius = (float)sqrt((float)srcSize.width * srcSize.width + srcSize.height * srcSize.height) / (float)strength;
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
        handle.AddKernel("", "", "lens_correction.cl", "lenscorrection_pln", vld, vgd, "")(srcPtr,
                                                                                           dstPtr,
                                                                                           strength,
                                                                                           zoom,
                                                                                           halfWidth,
                                                                                           halfHeight,
                                                                                           correctionRadius,
                                                                                           srcSize.height,
                                                                                           srcSize.width,
                                                                                           channel);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
        handle.AddKernel("", "", "lens_correction.cl", "lenscorrection_pkd", vld, vgd, "")(srcPtr,
                                                                                           dstPtr,
                                                                                           strength,
                                                                                           zoom,
                                                                                           halfWidth,
                                                                                           halfHeight,
                                                                                           correctionRadius,
                                                                                           srcSize.height,
                                                                                           srcSize.width,
                                                                                           channel);
    }
    else
    {
        std::cerr << "Internal error: Unknown Channel format";
    }
    return RPP_SUCCESS;
}

RppStatus
lens_correction_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
                         RppiChnFormat chnFormat, unsigned int channel)

{
    int plnpkdind;

    if(chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = 3;

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.csrcSize.height, handle.GetInitHandle()->mem.mgpu.csrcSize.width, handle.GetBatchSize(), &max_height, &max_width);

    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
    handle.AddKernel("", "", "lens_correction.cl", "lens_correction_batch", vld, vgd, "")(srcPtr,
                                                                                         dstPtr,
                                                                                        handle.GetInitHandle()->mem.mgpu.floatArr[0].floatmem,
                                                                                        handle.GetInitHandle()->mem.mgpu.floatArr[1].floatmem,
                                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,
                                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                                        channel,
                                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                                        plnpkdind);
    return RPP_SUCCESS;
}

/************* Flip ******************/

RppStatus
flip_cl(cl_mem srcPtr, RppiSize srcSize,
        cl_mem dstPtr, uint flipAxis,
        RppiChnFormat chnFormat, unsigned int channel,
        rpp::Handle &handle)

{
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        if (flipAxis == 1)
        {
            std::vector<size_t> vld{32, 32, 1};
            std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
            handle.AddKernel("", "", "flip.cl", "flip_vertical_planar", vld, vgd, "")(srcPtr,
                                                                                      dstPtr,
                                                                                      srcSize.height,
                                                                                      srcSize.width,
                                                                                      channel);
        }
        else if (flipAxis == 0)
        {
            std::vector<size_t> vld{32, 32, 1};
            std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
            handle.AddKernel("", "", "flip.cl", "flip_horizontal_planar", vld, vgd, "")(srcPtr,
                                                                                        dstPtr,
                                                                                        srcSize.height,
                                                                                        srcSize.width,
                                                                                        channel);
        }
        else if (flipAxis == 2)
        {
            std::vector<size_t> vld{32, 32, 1};
            std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
            handle.AddKernel("", "", "flip.cl", "flip_bothaxis_planar", vld, vgd, "")(srcPtr,
                                                                                      dstPtr,
                                                                                      srcSize.height,
                                                                                      srcSize.width,
                                                                                      channel);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        if (flipAxis == 1)
        {
            std::vector<size_t> vld{32, 32, 1};
            std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
            handle.AddKernel("", "", "flip.cl", "flip_vertical_packed", vld, vgd, "")(srcPtr,
                                                                                      dstPtr,
                                                                                      srcSize.height,
                                                                                      srcSize.width,
                                                                                      channel);
        }
        else if (flipAxis == 0)
        {
            std::vector<size_t> vld{32, 32, 1};
            std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
            handle.AddKernel("", "", "flip.cl", "flip_horizontal_packed", vld, vgd, "")(srcPtr,
                                                                                        dstPtr,
                                                                                        srcSize.height,
                                                                                        srcSize.width,
                                                                                        channel);
        }
        else if (flipAxis == 2)
        {
            std::vector<size_t> vld{32, 32, 1};
            std::vector<size_t> vgd{srcSize.width, srcSize.height, channel};
            handle.AddKernel("", "", "flip.cl", "flip_bothaxis_packed", vld, vgd, "")(srcPtr,
                                                                                      dstPtr,
                                                                                      srcSize.height,
                                                                                      srcSize.width,
                                                                                      channel);
        }
    }
    return RPP_SUCCESS;
}

RppStatus
flip_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
              RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;

    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = 3;
    

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.csrcSize.height, handle.GetInitHandle()->mem.mgpu.csrcSize.width, handle.GetBatchSize(), &max_height, &max_width);

    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
    //std::cout << "coming till near kernel here" << std::endl;
    handle.AddKernel("", "", "flip.cl", "flip_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[0].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        plnpkdind);


    return RPP_SUCCESS;
}

/************* Resize ******************/
RppStatus
resize_cl(cl_mem srcPtr, RppiSize srcSize,
          cl_mem dstPtr, RppiSize dstSize,
          RppiChnFormat chnFormat, unsigned int channel,
          rpp::Handle &handle)
{
    if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        //std::cout << dstSize.width << dstSize.height << std::endl;
        handle.AddKernel("", "", "resize.cl", "resize_pkd", vld, vgd, "")(srcPtr,
                                                                          dstPtr,
                                                                          srcSize.height,
                                                                          srcSize.width,
                                                                          dstSize.height,
                                                                          dstSize.width,
                                                                          channel);
    }
    else
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "resize.cl", "resize_pln", vld, vgd, "")(srcPtr,
                                                                          dstPtr,
                                                                          srcSize.height,
                                                                          srcSize.width,
                                                                          dstSize.height,
                                                                          dstSize.width,
                                                                          channel);
    }
    return RPP_SUCCESS;
}

RppStatus
resize_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
                RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;
    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = channel;
    unsigned int padding = 0;
    unsigned int type = 0;

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.cdstSize.height, handle.GetInitHandle()->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);

    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};

    handle.AddKernel("", "", "resize.cl", "resize_crop_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        padding,
                                                                        type,
                                                                        plnpkdind, plnpkdind);
    return RPP_SUCCESS;
}

/************* Resize Crop ******************/
RppStatus
resize_crop_cl(cl_mem srcPtr, RppiSize srcSize,
               cl_mem dstPtr, RppiSize dstSize,
               Rpp32u x1, Rpp32u x2, Rpp32u y1, Rpp32u y2,
               RppiChnFormat chnFormat, unsigned int channel,
               rpp::Handle &handle)
{
    unsigned int padding = 0;
    unsigned int type = 0;
    unsigned int width, height;
    if (type == 1)
    {
        width = dstSize.width - padding * 2;
        height = dstSize.height - padding * 2;
    }
    else
    {
        width = dstSize.width;
        height = dstSize.height;
    }
    if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{width, height, channel};
        handle.AddKernel("", "", "resize.cl", "resize_crop_pkd", vld, vgd, "")(srcPtr,
                                                                               dstPtr,
                                                                               srcSize.height,
                                                                               srcSize.width,
                                                                               height,
                                                                               width,
                                                                               x1,
                                                                               y1,
                                                                               x2,
                                                                               y2,
                                                                               padding,
                                                                               type,
                                                                               channel);
    }
    else
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{width, height, channel};
        handle.AddKernel("", "", "resize.cl", "resize_crop_pln", vld, vgd, "")(srcPtr,
                                                                               dstPtr,
                                                                               srcSize.height,
                                                                               srcSize.width,
                                                                               height,
                                                                               width,
                                                                               x1,
                                                                               y1,
                                                                               x2,
                                                                               y2,
                                                                               padding,
                                                                               type,
                                                                               channel);
    }
    return RPP_SUCCESS;
}
RppStatus
resize_crop_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
                     RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;
    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = channel;
    unsigned int padding = 10;
    unsigned int type = 1;

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.cdstSize.height, handle.GetInitHandle()->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
    handle.AddKernel("", "", "resize.cl", "resize_crop_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[0].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[1].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[2].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[3].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        padding,
                                                                        type,
                                                                        plnpkdind, plnpkdind);
    return RPP_SUCCESS;
}

RppStatus
rotate_cl_batch_tensor(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle, RPPTensorFunctionMetaData &tensor_info)
{
    int in_plnpkdind = getplnpkdind(tensor_info._in_format), out_plnpkdind = getplnpkdind(tensor_info._out_format);
    int batch_size = handle.GetBatchSize();
    InitHandle *handle_obj = handle.GetInitHandle();
    Rpp32u max_height, max_width;
    max_size(handle_obj->mem.mgpu.cdstSize.height, handle_obj->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    
    std::vector<size_t> vld{16, 16, 1};
    std::vector<size_t> vgd{max_width ,max_height , handle.GetBatchSize()};
    std::string kernel_file  = "rotate.cl";
    std::string kernel_name = "rotate_batch";
    get_kernel_name(kernel_name, tensor_info);
    std::cout << "in_plnindex " << in_plnpkdind  << "  out_plnpkdindex " << out_plnpkdind << std::endl;
    std::cout << "chaneeks " << tensor_info._in_channels  << std::endl;
    std::cout << kernel_name << std::endl;
    handle.AddKernel("", "", kernel_file, kernel_name, vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.floatArr[0].floatmem,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        tensor_info._in_channels,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        in_plnpkdind, out_plnpkdind);
    return RPP_SUCCESS;
}
RppStatus
resize_crop_cl_batch_tensor(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle, RPPTensorFunctionMetaData &tensor_info)
{
    unsigned int padding = 10;
    unsigned int type =  1;
    int in_plnpkdind = getplnpkdind(tensor_info._in_format), out_plnpkdind = getplnpkdind(tensor_info._out_format);
    int batch_size = handle.GetBatchSize();
    InitHandle *handle_obj = handle.GetInitHandle();
    Rpp32u max_height, max_width;
    max_size(handle_obj->mem.mgpu.cdstSize.height, handle_obj->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    
    std::vector<size_t> vld{16, 16, 1};
    std::vector<size_t> vgd{max_width ,max_height , handle.GetBatchSize()};
    std::string kernel_file  = "resize.cl";
    std::string kernel_name = "resize_crop_batch";
    get_kernel_name(kernel_name, tensor_info);
    std::cout << "in_plnindex " << in_plnpkdind  << "  out_plnpkdindex " << out_plnpkdind << std::endl;
    std::cout << kernel_name << std::endl;
    handle.AddKernel("", "", kernel_file, kernel_name, vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[0].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[1].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[2].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.uintArr[3].uintmem,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        tensor_info._in_channels,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        padding,
                                                                        type,
                                                                        in_plnpkdind, out_plnpkdind);
    return RPP_SUCCESS;
}

RppStatus
resize_cl_batch_tensor(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle, RPPTensorFunctionMetaData &tensor_info)
{
    unsigned int padding = 0;
    unsigned int type = 0;
    int in_plnpkdind = getplnpkdind(tensor_info._in_format), out_plnpkdind = getplnpkdind(tensor_info._out_format);
    int batch_size = handle.GetBatchSize();
    InitHandle *handle_obj = handle.GetInitHandle();
    Rpp32u max_height, max_width;
    max_size(handle_obj->mem.mgpu.cdstSize.height, handle_obj->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    
    std::vector<size_t> vld{16, 16, 1};
    std::vector<size_t> vgd{max_width ,max_height , handle.GetBatchSize()};
    std::string kernel_file  = "resize.cl";
    std::string kernel_name = "resize_crop_batch";
    get_kernel_name(kernel_name, tensor_info);
    std::cout << "in_plnindex " << in_plnpkdind  << "  out_plnpkdindex " << out_plnpkdind << std::endl;
    std::cout << kernel_name << std::endl;
    handle.AddKernel("", "", kernel_file, kernel_name, vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight, 
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        tensor_info._in_channels,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        padding,
                                                                        type,
                                                                        in_plnpkdind, out_plnpkdind);
    return RPP_SUCCESS;
}

/*************Rotate ******************/
RppStatus
rotate_cl(cl_mem srcPtr, RppiSize srcSize,
          cl_mem dstPtr, RppiSize dstSize, float angleDeg,
          RppiChnFormat chnFormat, unsigned int channel,
          rpp::Handle &handle)
{
    if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{(dstSize.width + 31) & ~31, (dstSize.height + 31) & ~31, channel};
        handle.AddKernel("", "", "rotate.cl", "rotate_pkd", vld, vgd, "")(srcPtr,
                                                                          dstPtr,
                                                                          angleDeg,
                                                                          srcSize.height,
                                                                          srcSize.width,
                                                                          dstSize.height,
                                                                          dstSize.width,
                                                                          channel);
    }
    else
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{(dstSize.width + 31) & ~31, (dstSize.height + 31) & ~31, channel};
        handle.AddKernel("", "", "rotate.cl", "rotate_pln", vld, vgd, "")(srcPtr,
                                                                          dstPtr,
                                                                          angleDeg,
                                                                          srcSize.height,
                                                                          srcSize.width,
                                                                          dstSize.height,
                                                                          dstSize.width,
                                                                          channel);
    }
    return RPP_SUCCESS;
}
RppStatus
rotate_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
                RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;

    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = channel;

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.cdstSize.height, handle.GetInitHandle()->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
        
    handle.AddKernel("", "", "rotate.cl", "rotate_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.floatArr[0].floatmem,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        plnpkdind, plnpkdind);
    return RPP_SUCCESS;
}

/************* Warp affine ******************/
RppStatus
warp_affine_cl(cl_mem srcPtr, RppiSize srcSize,
               cl_mem dstPtr, RppiSize dstSize, float *affine,
               RppiChnFormat chnFormat, unsigned int channel,
               rpp::Handle &handle)
{
    cl_int err;
    float affine_inv[6];
    float det; //for Deteminent
    det = (affine[0] * affine[4]) - (affine[1] * affine[3]);
    affine_inv[0] = affine[4] / det;
    affine_inv[1] = (-1 * affine[1]) / det;
    affine_inv[2] = -1 * affine[2];
    affine_inv[3] = (-1 * affine[3]) / det;
    affine_inv[4] = affine[0] / det;
    affine_inv[5] = -1 * affine[5];

    cl_kernel theKernel;
    cl_program theProgram;
    cl_context theContext;
    clGetCommandQueueInfo(handle.GetStream(),
                          CL_QUEUE_CONTEXT,
                          sizeof(cl_context), &theContext, NULL);
    cl_mem affine_array = clCreateBuffer(theContext, CL_MEM_READ_ONLY,
                                         sizeof(float) * 6, NULL, NULL);
    err = clEnqueueWriteBuffer(handle.GetStream(), affine_array, CL_TRUE, 0,
                               sizeof(float) * 6,
                               affine_inv, 0, NULL, NULL);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "warp_affine.cl", "warp_affine_pln", vld, vgd, "")(srcPtr,
                                                                                    dstPtr,
                                                                                    affine_array,
                                                                                    srcSize.height,
                                                                                    srcSize.width,
                                                                                    dstSize.height,
                                                                                    dstSize.width,
                                                                                    channel);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "warp_affine.cl", "warp_affine_pkd", vld, vgd, "")(srcPtr,
                                                                                    dstPtr,
                                                                                    affine_array,
                                                                                    srcSize.height,
                                                                                    srcSize.width,
                                                                                    dstSize.height,
                                                                                    dstSize.width,
                                                                                    channel);
    }

    else
    {
        std::cerr << "Internal error: Unknown Channel format";
    }

   
    return RPP_SUCCESS;
}

RppStatus
warp_affine_cl_batch(cl_mem srcPtr, cl_mem dstPtr,  rpp::Handle &handle, Rpp32f *affine,
                RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;

    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = 3;
    
    cl_int err;
    cl_context theContext;
    clGetCommandQueueInfo(handle.GetStream(),
                          CL_QUEUE_CONTEXT,
                          sizeof(cl_context), &theContext, NULL);
    cl_mem affine_array = clCreateBuffer(theContext, CL_MEM_READ_ONLY,
                                         sizeof(float) * 6 * handle.GetBatchSize(), NULL, NULL);
    err = clEnqueueWriteBuffer(handle.GetStream(), affine_array, CL_TRUE, 0,
                               sizeof(float) * 6 *handle.GetBatchSize(),
                               affine, 0, NULL, NULL);


    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.cdstSize.height, handle.GetInitHandle()->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
        
    handle.AddKernel("", "", "warp_affine.cl", "warp_affine_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        affine_array,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        plnpkdind);
    return RPP_SUCCESS;
}

/************* Warp Perspective ******************/
RppStatus
warp_perspective_cl(cl_mem srcPtr, RppiSize srcSize, cl_mem dstPtr,
                    RppiSize dstSize, float *perspective, RppiChnFormat chnFormat,
                    unsigned int channel, rpp::Handle &handle)
{
    cl_int err;
    float perspective_inv[9];
    float det; //for Deteminent
    det = (perspective[0] * ((perspective[4] * perspective[8]) - (perspective[5] * perspective[7]))) - (perspective[1] * ((perspective[3] * perspective[8]) - (perspective[5] * perspective[6]))) + (perspective[2] * ((perspective[3] * perspective[7]) - (perspective[4] * perspective[6])));
    perspective_inv[0] = (1 * ((perspective[4] * perspective[8]) - (perspective[5] * perspective[7]))) / det;
    perspective_inv[1] = (-1 * ((perspective[1] * perspective[8]) - (perspective[7] * perspective[2]))) / det;
    perspective_inv[2] = (1 * ((perspective[1] * perspective[5]) - (perspective[4] * perspective[2]))) / det;
    perspective_inv[3] = (-1 * ((perspective[3] * perspective[8]) - (perspective[6] * perspective[5]))) / det;
    perspective_inv[4] = (1 * ((perspective[0] * perspective[8]) - (perspective[6] * perspective[2]))) / det;
    perspective_inv[5] = (-1 * ((perspective[0] * perspective[5]) - (perspective[3] * perspective[2]))) / det;
    perspective_inv[6] = (1 * ((perspective[3] * perspective[7]) - (perspective[6] * perspective[4]))) / det;
    perspective_inv[7] = (-1 * ((perspective[0] * perspective[7]) - (perspective[6] * perspective[1]))) / det;
    perspective_inv[8] = (1 * ((perspective[0] * perspective[4]) - (perspective[3] * perspective[1]))) / det;

    float *perspective_matrix;
    cl_context theContext;
    clGetCommandQueueInfo(handle.GetStream(), CL_QUEUE_CONTEXT, sizeof(cl_context), &theContext, NULL);
    cl_mem perspective_array = clCreateBuffer(theContext, CL_MEM_READ_ONLY, sizeof(float) * 9, NULL, NULL);
    err = clEnqueueWriteBuffer(handle.GetStream(), perspective_array, CL_TRUE, 0, sizeof(float) * 9, perspective_inv, 0, NULL, NULL);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "warp_perspective.cl", "warp_perspective_pln", vld, vgd, "")(srcPtr,
                                                                                              dstPtr,
                                                                                              perspective_array,
                                                                                              srcSize.height,
                                                                                              srcSize.width,
                                                                                              dstSize.height,
                                                                                              dstSize.width,
                                                                                              channel);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "warp_perspective.cl", "warp_perspective_pln", vld, vgd, "")(srcPtr,
                                                                                              dstPtr,
                                                                                              perspective_array,
                                                                                              srcSize.height,
                                                                                              srcSize.width,
                                                                                              dstSize.height,
                                                                                              dstSize.width,
                                                                                              channel);
    }
    else
    {
        std::cerr << "Internal error: Unknown Channel format";
    }

    return RPP_SUCCESS;
}


RppStatus
warp_perspective_cl_batch(cl_mem srcPtr, cl_mem dstPtr,  rpp::Handle &handle,Rpp32f *perspective,   
                RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;

    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = 3;
    
    cl_int err;
    cl_context theContext;
    clGetCommandQueueInfo(handle.GetStream(),
                          CL_QUEUE_CONTEXT,
                          sizeof(cl_context), &theContext, NULL);
    cl_mem perspective_array = clCreateBuffer(theContext, CL_MEM_READ_ONLY,
                                         sizeof(float) * 9 * handle.GetBatchSize(), NULL, NULL);
    err = clEnqueueWriteBuffer(handle.GetStream(), perspective_array, CL_TRUE, 0,
                               sizeof(float) * 9 *handle.GetBatchSize(),
                               perspective, 0, NULL, NULL);


    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.cdstSize.height, handle.GetInitHandle()->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);
    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
        
    handle.AddKernel("", "", "warp_perspective.cl", "warp_perspective_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        perspective_array,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        plnpkdind);
    return RPP_SUCCESS;
}


// /************* Scale ******************/
RppStatus
scale_cl_batch(cl_mem srcPtr, cl_mem dstPtr, rpp::Handle &handle,
                RppiChnFormat chnFormat, unsigned int channel)
{
    int plnpkdind;

    if (chnFormat == RPPI_CHN_PLANAR)
        plnpkdind = 1;
    else
        plnpkdind = 3;
    unsigned int padding = 0;
    unsigned int type = 0;

    Rpp32u max_height, max_width;
    max_size(handle.GetInitHandle()->mem.mgpu.cdstSize.height, handle.GetInitHandle()->mem.mgpu.cdstSize.width, handle.GetBatchSize(), &max_height, &max_width);

    std::vector<size_t> vld{32, 32, 1};
    std::vector<size_t> vgd{max_width, max_height, handle.GetBatchSize()};
    //std::cout << "coming till here" << std::endl;
    handle.AddKernel("", "", "scale.cl", "scale_batch", vld, vgd, "")(srcPtr, dstPtr,
                                                                        handle.GetInitHandle()->mem.mgpu.floatArr[0].floatmem,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.srcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.height,
                                                                        handle.GetInitHandle()->mem.mgpu.dstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxSrcSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.maxDstSize.width,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.x,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiWidth,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.y,
                                                                        handle.GetInitHandle()->mem.mgpu.roiPoints.roiHeight,   
                                                                        handle.GetInitHandle()->mem.mgpu.srcBatchIndex,
                                                                        handle.GetInitHandle()->mem.mgpu.dstBatchIndex,
                                                                        channel,
                                                                        handle.GetInitHandle()->mem.mgpu.inc,
                                                                        handle.GetInitHandle()->mem.mgpu.dstInc,
                                                                        plnpkdind);
    return RPP_SUCCESS;
}

RppStatus
scale_cl(cl_mem srcPtr, RppiSize srcSize, cl_mem dstPtr, RppiSize dstSize,
         Rpp32f percentage, RppiChnFormat chnFormat, unsigned int channel, rpp::Handle &handle)
{
    percentage /= 100;
    unsigned int dstheight = (Rpp32s)(percentage * (Rpp32f)srcSize.height);
    unsigned int dstwidth = (Rpp32s)(percentage * (Rpp32f)srcSize.width);
    unsigned short counter = 0;
    cl_int err;
    cl_kernel theKernel;
    cl_program theProgram;
    cl_context theContext;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "scale.cl", "scale_pln", vld, vgd, "")(srcPtr,
                                                                        dstPtr,
                                                                        srcSize.height,
                                                                        srcSize.width,
                                                                        dstSize.height,
                                                                        dstSize.width,
                                                                        channel,
                                                                        dstheight,
                                                                        dstwidth);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        std::vector<size_t> vld{32, 32, 1};
        std::vector<size_t> vgd{dstSize.width, dstSize.height, channel};
        handle.AddKernel("", "", "scale.cl", "scale_pkd", vld, vgd, "")(srcPtr,
                                                                        dstPtr,
                                                                        srcSize.height,
                                                                        srcSize.width,
                                                                        dstSize.height,
                                                                        dstSize.width,
                                                                        channel,
                                                                        dstheight,
                                                                        dstwidth);
    }
    else
    {
        std::cerr << "Internal error: Unknown Channel format";
    }
    return RPP_SUCCESS;
}
