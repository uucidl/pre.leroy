/*
 * @url: https://gist.github.com/pervognsen/052f3062db9545b43a7e9350130cb964
 * @lang: c89
 * @taglist: concurrency
 */

typedef struct task_handle task_t;
typedef void (*task_function_t)(void *task_data);

/* Create a new task and returns its handle. A null handle (all bits to zero)
 * denotes an allocation error. */
task_t task_create(task_function_t task_function, void *task_data);

/* Mark that `dependency` depends on `task`. Upon completion of `task` all its
 * dependencies are started. */
void task_depends(task_t task, task_t dependency);

/* Schedule the task to run as soon as possible. */
void task_start(task_t task);

/* Name for a task. The null task has id = 0. */
struct task_handle {
    int id;
};

/*
 * Implementation notes from Per Vognsen:
 *
 * About the task_handle's id:
 * Lower bits as table index, upper bits as generation counter for
 * use-after-free detection
 *
 * Memory requirements for a minimal implementation: 16/24 bytes per task and 8
 * bytes per dependency edge.
 *
 * Even so, you still don't want to have one task instance for every array entry
 * in a gigabyte array.
 *
 *  Always assign task workloads that, on average, justify the overhead of
 * scheduling a task, however minimal that overhead may be.
 * The other major benefit of working on bigger chunks of data is obviously
 * coherent memory access. The only benefit of finer-grained tasks is better
 * load balancing (and you don't need too much granularity for that) or when
 * finer grained tasks lead to finer grainer dependencies, letting work start
 * much sooner than otherwise. Even if your data has a natural granularity (a
 * million entities), you might not want to kick off a task for each. Batch them
 * up!
 *
 * This is the equivalent of parallel_foreach and similar things, but adapted
 * for the characteristics of your data.
 *
 * Don't try to make batching the job of the task system. The right strategy is
 * tied up with your data design.
 */