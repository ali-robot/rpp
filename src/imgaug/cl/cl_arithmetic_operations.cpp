#include <cl/rpp_cl_common.hpp>
#include "cl_declarations.hpp"

/********* Absolute Difference *********/

RppStatus
absolute_difference_cl ( cl_mem srcPtr1,cl_mem srcPtr2,
                 RppiSize srcSize, cl_mem dstPtr,
                 RppiChnFormat chnFormat, unsigned int channel,
                 cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;
    CreateProgramFromBinary(theQueue,"absolute_difference.cl","absolute_difference.cl.bin","absolute_difference",theProgram,theKernel);
    // cl_kernel_initializer(theQueue,
    //                       "absolute_difference.cl",
    //                       "absolute_difference",
    //                       theProgram, theKernel);

    //---- Args Setter
    clRetainKernel(theKernel);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;

}

 /************* Arithmetic Add ************/

RppStatus
add_cl ( cl_mem srcPtr1,cl_mem srcPtr2,
                 RppiSize srcSize, cl_mem dstPtr,
                 RppiChnFormat chnFormat, unsigned int channel,
                 cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_int err;
    cl_kernel theKernel;
    cl_program theProgram;
    // clGetCommandQueueInfo(  theQueue,
    //                         CL_QUEUE_DEVICE, sizeof(cl_device_id), &theDevice, NULL);
    err = CreateProgramFromBinary(theQueue,"add.cl","add.cl.bin","add",theProgram,theKernel);
    // cl_kernel_initializer(theQueue,
    //                       "add.cl",
    //                       "add",
    //                       theProgram, theKernel);

    //---- Args Setter
    err = clRetainKernel(theKernel);
    err = clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    err |= clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    err |= clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    err |= clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &(srcSize.height));
    err |= clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &(srcSize.width));
    err |= clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &(channel));
    //----
    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;

}


/**************** Arithmetic Subtract *******************/
RppStatus
subtract_cl ( cl_mem srcPtr1,cl_mem srcPtr2,
                 RppiSize srcSize, cl_mem dstPtr,
                 RppiChnFormat chnFormat, unsigned int channel,
                 cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;

    CreateProgramFromBinary(theQueue,"subtract.cl","subtract.cl.bin","subtract",theProgram,theKernel);
    clRetainKernel(theKernel); 

    // cl_kernel_initializer(theQueue,
    //                       "subtract.cl",
    //                       "subtract",
    //                       theProgram, theKernel);

    //---- Args Setter
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;

}

/**************** Accumulate *******************/
RppStatus
accumulate_cl ( cl_mem srcPtr1,cl_mem srcPtr2,
                 RppiSize srcSize,
                 RppiChnFormat chnFormat, unsigned int channel,
                 cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;

    CreateProgramFromBinary(theQueue,"accumulate.cl","accumulate.cl.bin","accumulate",theProgram,theKernel);
    clRetainKernel(theKernel); 

    // cl_kernel_initializer(theQueue,
    //                       "accumulate.cl",
    //                       "accumulate",
    //                       theProgram, theKernel);

    //---- Args Setter
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;

}

/**************** Accumulate weighted *******************/

RppStatus
accumulate_weighted_cl ( cl_mem srcPtr1,cl_mem srcPtr2,
                 RppiSize srcSize, Rpp64f alpha,
                 RppiChnFormat chnFormat, unsigned int channel,
                 cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;

    CreateProgramFromBinary(theQueue,"accumulate.cl","accumulate.cl.bin","accumulate_weighted",theProgram,theKernel);
    clRetainKernel(theKernel); 

    // cl_kernel_initializer(theQueue,
    //                       "accumulate.cl",
    //                       "accumulate_weighted",
    //                       theProgram, theKernel);

    //---- Args Setter
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(Rpp64f), &alpha);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;

}

RppStatus
tensor_add_cl(Rpp32u tensorDimension, Rpp32u* tensorDimensionValues, cl_mem srcPtr1,cl_mem srcPtr2, cl_mem dstPtr, cl_command_queue theQueue)
{ 
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;

    CreateProgramFromBinary(theQueue,"tensor.cl","tensor.cl.bin","tensor_add",theProgram,theKernel);
    clRetainKernel(theKernel);
    
    size_t gDim3[3];
    if(tensorDimension == 1)
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = 1;
        gDim3[2] = 1;
    }
    else if(tensorDimension == 2)
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = tensorDimensionValues[1];
        gDim3[2] = 1;
    }
    else
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = tensorDimensionValues[1];
        int value = 1;
        for(int i = 2 ; i < tensorDimension ; i++)
        {    
            value *= tensorDimensionValues[i];
        }
        gDim3[2] = value;
    }
    
    unsigned int dim1,dim2,dim3;
    dim1 = gDim3[0];
    dim2 = gDim3[1];
    dim3 = gDim3[2];

    //---- Args Setter
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &tensorDimension);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim1);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim2);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim3);
    //----

    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);
    return RPP_SUCCESS;
}

RppStatus
tensor_subtract_cl(Rpp32u tensorDimension, Rpp32u* tensorDimensionValues, cl_mem srcPtr1,cl_mem srcPtr2, cl_mem dstPtr, cl_command_queue theQueue)
{ 
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;

    CreateProgramFromBinary(theQueue,"tensor.cl","tensor.cl.bin","tensor_subtract",theProgram,theKernel);
    clRetainKernel(theKernel);
    
    size_t gDim3[3];
    if(tensorDimension == 1)
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = 1;
        gDim3[2] = 1;
    }
    else if(tensorDimension == 2)
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = tensorDimensionValues[1];
        gDim3[2] = 1;
    }
    else
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = tensorDimensionValues[1];
        int value = 1;
        for(int i = 2 ; i < tensorDimension ; i++)
        {    
            value *= tensorDimensionValues[i];
        }
        gDim3[2] = value;
    }
    
    unsigned int dim1,dim2,dim3;
    dim1 = gDim3[0];
    dim2 = gDim3[1];
    dim3 = gDim3[2];

    //---- Args Setter
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &tensorDimension);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim1);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim2);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim3);
    //----

    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);
    return RPP_SUCCESS;
}

RppStatus
tensor_multiply_cl(Rpp32u tensorDimension, Rpp32u* tensorDimensionValues, cl_mem srcPtr1,cl_mem srcPtr2, cl_mem dstPtr, cl_command_queue theQueue)
{ 
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;

    CreateProgramFromBinary(theQueue,"tensor.cl","tensor.cl.bin","tensor_multiply",theProgram,theKernel);
    clRetainKernel(theKernel);
    
    size_t gDim3[3];
    if(tensorDimension == 1)
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = 1;
        gDim3[2] = 1;
    }
    else if(tensorDimension == 2)
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = tensorDimensionValues[1];
        gDim3[2] = 1;
    }
    else
    {
        gDim3[0] = tensorDimensionValues[0];
        gDim3[1] = tensorDimensionValues[1];
        int value = 1;
        for(int i = 2 ; i < tensorDimension ; i++)
        {    
            value *= tensorDimensionValues[i];
        }
        gDim3[2] = value;
    }
    
    unsigned int dim1,dim2,dim3;
    dim1 = gDim3[0];
    dim2 = gDim3[1];
    dim3 = gDim3[2];

    //---- Args Setter
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &tensorDimension);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim1);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim2);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &dim3);
    //----

    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);
    return RPP_SUCCESS;    
}

RppStatus
multiply_cl( cl_mem srcPtr1,cl_mem srcPtr2, RppiSize srcSize, cl_mem dstPtr, RppiChnFormat chnFormat, unsigned int channel, cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;
    CreateProgramFromBinary(theQueue,"multiply.cl","multiply.cl.bin","multiply",theProgram,theKernel);
    //---- Args Setter
    clRetainKernel(theKernel);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;
}

RppStatus
magnitude_cl( cl_mem srcPtr1,cl_mem srcPtr2, RppiSize srcSize, cl_mem dstPtr, RppiChnFormat chnFormat, unsigned int channel, cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;
    CreateProgramFromBinary(theQueue,"magnitude.cl","magnitude.cl.bin","magnitude",theProgram,theKernel);
    //---- Args Setter
    clRetainKernel(theKernel);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;
}

RppStatus
min_cl( cl_mem srcPtr1,cl_mem srcPtr2, RppiSize srcSize, cl_mem dstPtr, RppiChnFormat chnFormat, unsigned int channel, cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;
    CreateProgramFromBinary(theQueue,"min.cl","min.cl.bin","min",theProgram,theKernel);
    //---- Args Setter
    clRetainKernel(theKernel);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;
}

RppStatus
max_cl( cl_mem srcPtr1,cl_mem srcPtr2, RppiSize srcSize, cl_mem dstPtr, RppiChnFormat chnFormat, unsigned int channel, cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;
    CreateProgramFromBinary(theQueue,"max.cl","max.cl.bin","max",theProgram,theKernel);
    //---- Args Setter
    clRetainKernel(theKernel);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;
}

RppStatus
phase_cl( cl_mem srcPtr1,cl_mem srcPtr2, RppiSize srcSize, cl_mem dstPtr, RppiChnFormat chnFormat, unsigned int channel, cl_command_queue theQueue)
{
    unsigned short counter=0;
    cl_kernel theKernel;
    cl_program theProgram;
    CreateProgramFromBinary(theQueue,"phase.cl","phase.cl.bin","phase",theProgram,theKernel);
    //---- Args Setter
    clRetainKernel(theKernel);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr1);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &srcPtr2);
    clSetKernelArg(theKernel, counter++, sizeof(cl_mem), &dstPtr);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.height);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &srcSize.width);
    clSetKernelArg(theKernel, counter++, sizeof(unsigned int), &channel);
    //----

    size_t gDim3[3];
    gDim3[0] = srcSize.width;
    gDim3[1] = srcSize.height;
    gDim3[2] = channel;
    cl_kernel_implementer (theQueue, gDim3, NULL/*Local*/, theProgram, theKernel);

    return RPP_SUCCESS;
}