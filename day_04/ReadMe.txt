
Step 1: find the unused/free major number from cat /proc/devices

Step 2:dynamic major and minor, while you insmod the module, it will display the major and minor, check with dmesg | tail

Step 3: mknod /dev/sample_drver c major minor , check the test.c node

step 4: change permissons /dev/sample_driver , to run the test app

Step 5: run the app, see the output/observations
