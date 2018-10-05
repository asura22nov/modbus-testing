#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main()
{

    modbus_t *ctx;
    uint16_t tab_reg[2] = {0,0};
    float avgVLL = -1;
    int res = 0;
    int rc;
    int i;
    struct timeval response_timeout;
    uint32_t tv_sec = 0;
    uint32_t tv_usec = 0;
    response_timeout.tv_sec = 5;
    response_timeout.tv_usec = 0;
    int regval;
    char regname[500];

	//--------set device communication-----------------------------
    //ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'E', 8, 1);
    ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
    if (NULL == ctx)
    {
                    printf("Unable to create libmodbus context\n");
                    res = 1;
    }
    else
    {
        printf("created libmodbus context\n");

        modbus_set_debug(ctx, TRUE);
        modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK |MODBUS_ERROR_RECOVERY_PROTOCOL);
		//--------------------------set device ID--------------------------------------------
        rc = modbus_set_slave(ctx, 1);
		//rc = modbus_set_slave(ctx, 2);
		printf("modbus_set_slave return: %d\n",rc);

        if (rc != 0)
        {
                        printf("modbus_set_slave: %s \n",modbus_strerror(errno));
        }

        /*
        Commented - Giving 'Bad File Descriptor' issue
        rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
        printf("modbus_rtu_set_serial_mode: %d \n",rc);

        if (rc != 0)
        {
                        printf("modbus_rtu_set_serial_mode: %s \n",modbus_strerror(errno));
        }
        */

        /*
        Commented - Giving 'Bad File Descriptor' issue
        rc = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
        printf("modbus_rtu_set_serial_mode: %d \n",rc);

        if (rc != 0)
        {
                        printf("modbus_rtu_set_serial_mode: %s \n",modbus_strerror(errno));
        }
        */

        /*
        // This code is for version 3.0.6

        modbus_get_response_timeout(ctx, &response_timeout);

        printf("Default response timeout:%ld sec %ld usec \n", response_timeout.tv_sec, response_timeout.tv_usec );

        response_timeout.tv_sec = 60;
        response_timeout.tv_usec = 0;

        modbus_set_response_timeout(ctx, &response_timeout);
        modbus_get_response_timeout(ctx, &response_timeout);

        printf("Set response timeout:%ld sec %ld usec \n", response_timeout.tv_sec, response_timeout.tv_usec );
        */


        //This code is for version 3.1.2
        modbus_get_response_timeout(ctx, &tv_sec, &tv_usec);
        printf("Default response timeout:%d sec %d usec \n",tv_sec,tv_usec );

        tv_sec = 60;
        tv_usec = 0;

        modbus_set_response_timeout(ctx, tv_sec,tv_usec);
        modbus_get_response_timeout(ctx, &tv_sec, &tv_usec);
        printf("Set response timeout:%d sec %d usec \n",tv_sec,tv_usec );


        rc = modbus_connect(ctx);

        printf("modbus_connect: %d \n",rc);

        if (rc == -1) {
                        printf("Connection failed: %s\n", modbus_strerror(errno));
            res = 1;
        }


      FILE *fp=fopen("/home/Modbus-code/Registry-number.txt","r"); //open file in read mode

       while (fscanf(fp, "%[^,],%d\n", regname, &regval) == 2) {
          //while (fscanf(fp, "%[^,]%d", name, &n) == 2) {
           // Do something with the data you just read:
           //printf("name='%s' int=%d\n",name, n);
           //printf("%s %d\n",regname, regval);
           rc = modbus_read_registers(ctx,regval,2,tab_reg);
           printf("%s=%f\n",regname,modbus_get_float(tab_reg));
       }
       fclose(fp);

        if (rc == -1) {
                        printf("Read registers failed:  %s\n", modbus_strerror(errno));
            res = 1;
        }
        modbus_close(ctx);
        modbus_free(ctx);
    }
}


