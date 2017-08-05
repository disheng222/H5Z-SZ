/**
 *  @file H5Z_SZ.h
 *  @author Sheng Di
 *  @date July, 2017
 *  @brief Header file for the H5Z_SZ.c.
 *  (C) 2015 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef _H5Z_SZ
#define _H5Z_SZ

#include <stdio.h>
#include "sz.h"

#define H5Z_FILTER_SZ 32017

#ifdef __cplusplus
extern "C" {
#endif

int H5Z_SZ_Init(char* cfgFile);
int H5Z_SZ_Init_Params(sz_params *params);
sz_params* H5Z_SZ_Init_Default();
int H5Z_SZ_Finalize();

void SZ_cdArrayToMetaData(size_t cd_nelmts, const unsigned int cd_values[], int* dimSize, int* dataType, size_t* r5, size_t* r4, size_t* r3, size_t* r2, size_t* r1);
void SZ_metaDataToCdArray(size_t* cd_nelmts, unsigned int** cd_values, int dataType, size_t r5, size_t r4, size_t r3, size_t r2, size_t r1);

static size_t H5Z_filter_sz(unsigned int flags, size_t cd_nelmts, const unsigned int cd_values[], size_t nbytes, size_t* buf_size, void** buf);

#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef _H5Z_SZ_metadata  ----- */
