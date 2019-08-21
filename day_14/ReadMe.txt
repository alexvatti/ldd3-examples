
This Example Moulde will run on S3C240 SoC: Pls Make a Note of It.

Step 1: find the unused/free major number from cat /proc/devices

Step 2: open the ex3.c fill with major and minor number, build, insmod (in case of static) 
 (in case of dynamic major and minor, while insmod, it will display the major and minor, check with dmesg | tail)

Step 3: mknod /dev/sample_driver c major minor , check the test.c node

step 4: change permissons /dev/sample_driver , to run the test app

Step 5: run the app, see the output/observations
