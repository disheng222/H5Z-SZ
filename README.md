# H5Z-SZ
hdf5 filter based on SZ compressor

#Quick start

Step 1: Download SZ package from : https://github.com/disheng222/SZ

$ e.g., git clone https://github.com/disheng222/SZ

Step 2: Simply install SZ by using "./configure --prefix=[INSTALL_PATH];make;make install"

$ e.g., ./configure --prefix=/home/sdi/Install/sz-1.4.10-beta-install

Step 3: You can go to example/ directory to test whether the installation is fine by the following commend: ./test.sh

Step 4: Download the H5Z_SZ_filter.

Step 5: Open Makefile and modify the SZ path and HDF5 path based on your local setting:
SZPATH          = /home/sdi/Install/sz-1.4.10-beta-install
HDF5PATH        = /home/sdi/Install/hdf5-1.10.1-install

Step 6: Execute "make;make install"

Step 7: Then, go to the ./test/ directory, and edit its Makefile by setting SZPATH and HDF5PATH similarly.

Step 8: make

Step 9: Set the environment variable: LD_LIBRARY_PATH=$HDF5_HOME/lib:$SZ_HOME/lib:$LD_LIBRARY_PATH

Step 10: Run test_compress.sh and test_decompress.sh to do the test (testing data is in the directory testdata/ of the package).


Contact:
sdi1@anl.gov
