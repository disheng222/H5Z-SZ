/**
 *  @file dszFromHDF5.c
 *  @author Sheng Di
 *  @date July, 2017
 *  @brief This is an example of using decompression interface (HDF5)
 *  (C) 2017 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "hdf5.h"
#include "sz.h"
#include "H5Z_SZ.h"

#define DATASET "testdata_compressed"
#define MAX_CHUNK_SIZE 4294967295 //2^32-1

int main(int argc, char * argv[])
{
	int dimSize = 0, dataType = 0;
	size_t r5=0,r4=0,r3=0,r2=0,r1=0,nbEle = 0;
	char hdf5FilePath[640], outputFilePath[640];
	hid_t file, dset, dcpl, space_id; /*Handles*/
	H5Z_filter_t filter_id = 0;
	herr_t status;

	htri_t avail;
	char filter_name[80];
	unsigned int flags = 0;
	size_t nelmts = 0;
	unsigned int values_out[7] = {0,0,0,0,0,0,0}; //at most 7 parameters 
	float* data = NULL;

	if(argc < 3)
	{
		printf("Test case: dszFromHDF5 [datatype] [hdf5FilePath]\n");
		printf("Example: szToHDF5 -f/-d testdata/x86/testfloat_8_8_128.dat.sz.hdf5\n");
		exit(0);
	}

	dataType = strcmp(argv[1],"-f")==0 ? 0: 1;
	sprintf(hdf5FilePath, "%s", argv[2]);
	sprintf(outputFilePath, "%s.out.h5", hdf5FilePath);

	/*Open the hdf5 file with SZ-compressed data*/
    file = H5Fopen(hdf5FilePath, H5F_ACC_RDONLY, H5P_DEFAULT);
    dset = H5Dopen(file, DATASET, H5P_DEFAULT);
    
    /*Retrieve dataset creation property list.*/
    dcpl = H5Dget_create_plist(dset);
    
    /*Retrieve and print the filter id, compression level and filter's name for SZ*/
    /*filter_id = H5Pget_filter2(dcpl, (unsigned)0, &flags, &nelmts, values_out, sizeof(filter_name), filter_name, NULL);
    printf("values_out[0]=%u, values_out[1]=%u\n", values_out[0], values_out[1]);
    printf(" Filter identifier is: %s, id = %d\n", filter_name, filter_id);
    
    switch(filter_id)
    {
	case H5Z_FILTER_SZ:
		SZ_cdArrayToMetaData(nelmts, values_out, &dimSize, &dataType, &r5, &r4, &r3, &r2, &r1);
		if(dataType==SZ_FLOAT)
			printf("data type: FLOAT\n");
		else
			printf("data type: DOUBLE\n");
		break;
	default:
		printf("Not expected filter\n");
		exit(0);
	}*/
	
    /*Check that filter is not registered with the library yet*/
	avail = H5Zfilter_avail(H5Z_FILTER_SZ);
	if(!avail)
		printf("sz filter is not yet available after the H5Pget_filter call.\n");
	else
		printf("sz filter is available.\n");
	
	space_id = H5Dget_space(dset);	
	nbEle = H5Sget_simple_extent_npoints(space_id);
	data = (float*)malloc(sizeof(float)*nbEle);	
	/*Read the data using the default properties.*/
	printf("....Reading SZ compressed data .....................\n");
	
	if(dataType==SZ_FLOAT)
	{
		status = H5Dread(dset, H5T_IEEE_F32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
	}
	else
	{
		status = H5Dread(dset, H5T_IEEE_F64LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
	}
	/*Print the first 10 data values to check the correctness.*/	
	int i;
	printf("reconstructed data = ");
	for(i=0;i<20;i++)
		printf("%f ", data[i]);	
	printf("\n");	
	
	free(data);
	
	status = H5Pclose(dcpl);
	status = H5Dclose(dset);
	status = H5Fclose(file);
	
	return 0;
}
