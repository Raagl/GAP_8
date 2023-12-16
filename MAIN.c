/* PMSIS includes */

#include "pmsis.h"

#include "NN.h"





#include <stdio.h>

#include <stdlib.h>

#include <math.h>





//define neural network parameters

#define INPUT_SIZE 2

#define OUTPUT_SIZE 1

#define LEARNING_RATE 0.01

#define EPOCHS 10000



#define RAN_NUM  3883838

#define RAN_MAX  RAN_NUM + 1378



// Performance Monitoring Event IDs

#define PI_PERF_CYCLES      0

#define PI_PERF_ACTIVE_CYCLES 1

#define PI_PERF_INSTR       2

#define PI_PERF_LD_STALL    3

#define PI_PERF_JR_STALL    4



uint32_t cycles, active_cycles, instructions, ld_stall, jr_stall;



/* Task executed by cluster cores. */

void cluster_helloworld(void *arg)

{

    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();

    // initialize in core 1

    if (core_id == 1)

    {

	 printf("initialize in core 1");

	 initialize();  

    }

    

    // training in core 2

    if (core_id == 2)

    {

    	printf("training in core 2\n");

    	printf("please wait \n");

    	training_process();

    	

    	printf("prediction result in core 2 \n");

    	printf("");

    	prediction_result();

    	 // Stop timing and update performance counters

            

            cycles += pi_perf_read(PI_PERF_CYCLES);

            active_cycles += pi_perf_read(PI_PERF_ACTIVE_CYCLES);

            instructions += pi_perf_read(PI_PERF_INSTR);

            ld_stall += pi_perf_read(PI_PERF_LD_STALL);

            jr_stall += pi_perf_read(PI_PERF_JR_STALL);

            

    }
    // Print performance results

    printf("Total cycles: %u\n", cycles);

    printf("Active cycles: %u\n", active_cycles);

    printf("Instructions executed: %u\n", instructions);

    printf("Load data hazards: %u\n", ld_stall);

    printf("Jump register data hazards: %u\n", jr_stall);
    



     

}



/* Cluster main entry, executed by core 0. */

void cluster_delegate(void *arg)

{

    printf("Cluster master core entry\n");

    /* Task dispatch to cluster cores. */

    pi_cl_team_fork(pi_cl_cluster_nb_cores(), cluster_helloworld, arg);

   

    

    printf("Cluster master core exit\n");

}



void helloworld(void)

{

    printf("Entering main controller\n");



    uint32_t errors = 0;

    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();

   

    struct pi_device cluster_dev;

    struct pi_cluster_conf cl_conf;



    /* Init cluster configuration structure. */

    pi_cluster_conf_init(&cl_conf);

    cl_conf.id = 0;                /* Set cluster ID. */

    /* Configure & open cluster. */

    pi_open_from_conf(&cluster_dev, &cl_conf);



    pi_perf_conf(1 << PI_PERF_CYCLES | 1 << PI_PERF_ACTIVE_CYCLES |1 << PI_PERF_INSTR |

                 1 << PI_PERF_LD_STALL | 1 << PI_PERF_JR_STALL);

    pi_perf_reset();

    pi_perf_start();

    

    



    if (pi_cluster_open(&cluster_dev))

    {

        printf("Cluster open failed !\n");

        pmsis_exit(-1);

    }



    pi_perf_stop();

    uint32_t cycles = pi_perf_read(PI_PERF_ACTIVE_CYCLES);

    uint32_t tim_cycles = pi_perf_read(PI_PERF_CYCLES);

    printf("Perf : %d cycles Timer : %d cycles\n", cycles, tim_cycles);



    /* Prepare cluster task and send it to cluster. */

    struct pi_cluster_task cl_task;



    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cl_task, cluster_delegate, NULL));



    pi_cluster_close(&cluster_dev);



    printf("Test success !\n");



    pmsis_exit(errors);

}



/* Program Entry. */

int main(void)

{

    printf("\n\n\t *** PMSIS HelloWorld ***\n\n");

    return pmsis_kickoff((void *) helloworld);

    

}
