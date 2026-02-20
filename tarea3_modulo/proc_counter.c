#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched/signal.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ulises Viña Almeida <ulisesv@ciencias.unam.mx>; Andrea Figueroa <andreafigueroa04@ciencias.unam.mx>");
MODULE_DESCRIPTION("Módulo que cuenta el número de procesos y programas únicos cada 30 segundos");

#define INTERVAL (30 * HZ)
#define MAX_PROGRAMS 512

static struct timer_list proc_timer;

static char programs[MAX_PROGRAMS][TASK_COMM_LEN];

static void count_programs(struct timer_list *t)
{
    struct task_struct *task;
    int total_processes = 0;
    int unique_programs = 0;
    int i, found;

    memset(programs, 0, sizeof(programs));

    for_each_process(task) {
        total_processes++;

        found = 0;
        for (i = 0; i < unique_programs; i++) {
            if (strncmp(programs[i], task->comm, TASK_COMM_LEN) == 0) {
                found = 1;
                break;
            }
        }

        if (!found && unique_programs < MAX_PROGRAMS) {
            strncpy(programs[unique_programs], task->comm, TASK_COMM_LEN);
            unique_programs++;
        }
    }

    printk(KERN_INFO "[proc_counter] Procesos Totales: %d | Programas Únicos: %d\n",
           total_processes, unique_programs);

    mod_timer(&proc_timer, jiffies + INTERVAL);
}

static int __init proc_counter_init(void)
{
    printk(KERN_INFO "Contador de Procesos Cargado\n");

    timer_setup(&proc_timer, count_programs, 0);
    mod_timer(&proc_timer, jiffies + INTERVAL);

    return 0;
}

static void __exit proc_counter_exit(void)
{
    timer_delete_sync(&proc_timer);
    printk(KERN_INFO "Contador de Procesos Deshabilitado\n");
}

module_init(proc_counter_init);
module_exit(proc_counter_exit);
