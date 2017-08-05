/**
 *  @file H5Z_SZ.c
 *  @author Sheng Di
 *  @date July, 2017
 *  @brief SZ filter for HDF5
 *  (C) 2017 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "H5Z_SZ.h"
#include "H5PLextern.h"

//sz_params* conf_params = NULL;

const H5Z_class2_t H5Z_SZ[1] = {{
	H5Z_CLASS_T_VERS,              /* H5Z_class_t version */
	(H5Z_filter_t)H5Z_FILTER_SZ, /* Filter id number */
	1,              /* encoder_present flag (set to true) */
	1,              /* decoder_present flag (set to true) */
	"SZ compressor/decompressor for floating-point data.", /* Filter name for debugging */
	NULL,                          /* The "can apply" callback */
	NULL,                          /* The "set local" callback */
	(H5Z_func_t)H5Z_filter_sz,   /* The actual filter function */
}};

H5PL_type_t H5PLget_plugin_type(void) {return H5PL_TYPE_FILTER;}
const void *H5PLget_plugin_info(void) {return H5Z_SZ;}

int H5Z_SZ_Init(char* cfgFile) 
{ 
	herr_t ret = H5Zregister(H5Z_SZ); 
	int status = SZ_Init(cfgFile);
	if(status == SZ_NSCS || ret < 0)
		return SZ_NSCS;
	else
		return SZ_SCES;
}

int H5Z_SZ_Init_Params(sz_params *params) 
{ 
	herr_t ret = H5Zregister(H5Z_SZ); 
	int status = SZ_Init_Params(params);
	if(status == SZ_NSCS || ret < 0)
		return SZ_NSCS;
	else
		return SZ_SCES;
}

sz_params* H5Z_SZ_Init_Default()
{
	herr_t ret = H5Zregister(H5Z_SZ);	
	
	sz_params* conf_params = (sz_params *)malloc(sizeof(sz_params));
	conf_params->quantization_intervals = 0;
	conf_params->max_quant_intervals = 65536;
    conf_params->dataEndianType = LITTLE_ENDIAN_DATA;
    conf_params->sol_ID = SZ;
    conf_params->layers = 1;
    conf_params->sampleDistance = 100;
    conf_params->predThreshold = 0.99;
    conf_params->offset = 0;
    conf_params->szMode = SZ_BEST_COMPRESSION;
    conf_params->gzipMode = 1; //best speed
    conf_params->errorBoundMode = ABS; //details about errorBoundMode can be found in sz.config
    conf_params->absErrBound = 1E-4;
    conf_params->relBoundRatio = 1E-4;
    conf_params->pw_relBoundRatio = 1E-4;
    conf_params->segment_size = 32;
    conf_params->pwr_type = SZ_PWR_AVG_TYPE;	
	
	int status = SZ_Init_Params(conf_params);
	if(status == SZ_NSCS || ret < 0)
		return NULL;
	else
		return conf_params;
}

int H5Z_SZ_Finalize()
{
	SZ_Finalize();
	herr_t ret = H5Zunregister(H5Z_FILTER_SZ);
	if (ret < 0) return -1;
	return 0;
}

/**
 * to be used in decompression and compression, inside the H5Z_filter_sz().
 * */
void SZ_cdArrayToMetaData(size_t cd_nelmts, const unsigned int cd_values[], int* dimSize, int* dataType, size_t* r5, size_t* r4, size_t* r3, size_t* r2, size_t* r1)
{
	assert(cd_nelmts >= 4);
	unsigned char bytes[8];	
	*dimSize = cd_values[0];
	*dataType = cd_values[1];
	switch(*dimSize)
	{
	case 1:
		intToBytes_bigEndian(bytes, cd_values[2]);
		intToBytes_bigEndian(&bytes[4], cd_values[3]);
		if(sizeof(size_t)==4)
			*r1 = (unsigned int)bytesToLong_bigEndian(bytes);
		else
			*r1 = (unsigned long)bytesToLong_bigEndian(bytes);
		*r2 = *r3 = *r4 = *r5 = 0;
		break;
	case 2:
		*r3 = *r4 = *r5 = 0;
		*r2 = cd_values[2];
		*r1 = cd_values[3];
		break;
	case 3:
		*r4 = *r5 = 0;
		*r3 = cd_values[2];
		*r2 = cd_values[3];
		*r1 = cd_values[4];
		break;
	case 4:
		*r5 = 0;
		*r4 = cd_values[2];
		*r3 = cd_values[3];
		*r2 = cd_values[4];
		*r1 = cd_values[5];	
		break;
	default: 
		*r5 = cd_values[2];
		*r4 = cd_values[3];
		*r3 = cd_values[4];
		*r2 = cd_values[5];
		*r1 = cd_values[6];		
	}
}

/**
 * to be used in compression, and to be called outside H5Z_filter_sz().
 * */
void SZ_metaDataToCdArray(size_t* cd_nelmts, unsigned int **cd_values, int dataType, size_t r5, size_t r4, size_t r3, size_t r2, size_t r1)
{
	unsigned char bytes[8] = {0};
	unsigned long size;
	*cd_values = (unsigned int*)malloc(sizeof(unsigned int)*7);
	int dim = computeDimension(r5, r4, r3, r2, r1);
	(*cd_values)[0] = dim;
	(*cd_values)[1] = dataType;	//0: FLOAT ; 1: DOUBLE
	switch(dim)
	{
	case 1:
		size = (unsigned long)r1;
		longToBytes_bigEndian(bytes, size);
		(*cd_values)[2] = bytesToInt_bigEndian(bytes);
		(*cd_values)[3] = bytesToInt_bigEndian(&bytes[4]);	
		*cd_nelmts = 4;
		break;
	case 2:
		(*cd_values)[2] = (unsigned int) r2;
		(*cd_values)[3] = (unsigned int) r1;	
		*cd_nelmts = 4;
		break;
	case 3:
		(*cd_values)[2] = (unsigned int) r3;
		(*cd_values)[3] = (unsigned int) r2;
		(*cd_values)[4] = (unsigned int) r1;	
		*cd_nelmts = 5;
		break;
	case 4:
		(*cd_values)[2] = (unsigned int) r4;	
		(*cd_values)[3] = (unsigned int) r3;
		(*cd_values)[4] = (unsigned int) r2;
		(*cd_values)[5] = (unsigned int) r1;	
		*cd_nelmts = 6;
		break;
	default:
		(*cd_values)[2] = (unsigned int) r5;		
		(*cd_values)[3] = (unsigned int) r4;	
		(*cd_values)[4] = (unsigned int) r3;
		(*cd_values)[5] = (unsigned int) r2;
		(*cd_values)[6] = (unsigned int) r1;
		*cd_nelmts = 7;	
	}
}


static size_t H5Z_filter_sz(unsigned int flags, size_t cd_nelmts, const unsigned int cd_values[], size_t nbytes, size_t* buf_size, void** buf)
{
	size_t r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
	int dimSize = 0, dataType = 0;
	SZ_cdArrayToMetaData(cd_nelmts, cd_values, &dimSize, &dataType, &r5, &r4, &r3, &r2, &r1);
	
/*	int i=0;
	for(i=0;i<cd_nelmts;i++)
		printf("cd_values[%d]=%u\n", i, cd_values[i]);
	printf("dimSize=%d, r1=%u, r2=%u, r3=%u, r4=%u, r5=%u\n", dimSize, r1, r2, r3, r4, r5);*/
	size_t nbEle = computeDataLength(r5, r4, r3, r2, r1); 
	
	if (flags & H5Z_FLAG_REVERSE) 
	{  
		/* decompress data */
		if(dataType == SZ_FLOAT)
		{
			float* data = SZ_decompress(SZ_FLOAT, *buf, nbytes, r5, r4, r3, r2, r1);
										
			free(*buf);
			*buf = data;
			*buf_size = nbEle*sizeof(float);
			return *buf_size;
		}
		else //==8
		{
			double* data = SZ_decompress(SZ_DOUBLE, *buf, nbytes, r5, r4, r3, r2, r1);
			free(*buf);
			*buf = data;
			*buf_size = nbEle*sizeof(double);			
			return *buf_size;
		}
	}
	else
	{
		size_t outSize = 0;
		if(dataType == SZ_FLOAT)
		{
			float* data = (float*)(*buf);
			unsigned char *bytes = SZ_compress(SZ_FLOAT, data, &outSize, r5, r4, r3, r2, r1);
			free(*buf);
			*buf = bytes;
			*buf_size = outSize;
			return outSize;
		}
		else //==1
		{
			double* data = (double*)(*buf);
			unsigned char *bytes = SZ_compress(SZ_DOUBLE, data, &outSize, r5, r4, r3, r2, r1);
			free(*buf);
			*buf = bytes;
			*buf_size = outSize;
			return outSize;	
		}
	}
	
}

