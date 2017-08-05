## * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
##   Makefile to test SZ library.

##=======================================================================
##   PLEASE SET THESE VARIABLES BEFORE COMPILING
##=======================================================================

SZPATH		= /home/sdi/Install/sz-1.4.10-beta-install
HDF5PATH	= /home/sdi/Install/hdf5-1.10.1-install

##   COMPILERS
CC		= gcc
MPICC 		= mpicc
MPIRUN 		= mpirun

##=======================================================================
##   FLAGS
##=======================================================================

#SZFLAGS		= -I$(SZPATH)/include $(SZPATH)/lib/libsz.a $(SZPATH)/lib/libzlib.a $(SZPATH)/lib/libhdf5sz.a
SZFLAGS		= -I$(SZPATH)/include -L$(SZPATH)/lib -L$(SZPATH)/lib

#HDF5FLAGS	= -I$(HDF5PATH)/include $(HDF5PATH)/lib/libhdf5.a
HDF5FLAGS	= -I$(HDF5PATH)/include -L$(HDF5PATH)/lib

##=======================================================================
##   TARGETS
##=======================================================================
all: 		szToHDF5 dszFromHDF5

szToHDF5:	szToHDF5.c
	$(CC)  -g -O0 -o szToHDF5 szToHDF5.c $(SZFLAGS) $(HDF5FLAGS) -lsz -lhdf5sz -lzlib -lhdf5 -lm -Wl,--no-as-needed -ldl

dszFromHDF5:	dszFromHDF5.c
	$(CC)  -g -O3 -o dszFromHDF5 dszFromHDF5.c $(SZFLAGS) $(HDF5FLAGS) -lsz -lhdf5sz -lzlib -lhdf5 -lm -Wl,--no-as-needed -ldl

clean:
	rm -f szToHDF5 dszFromHDF5

