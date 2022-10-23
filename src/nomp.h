#if !defined(_LIB_NOMP_H_)
#define _LIB_NOMP_H_

#include <stddef.h>

/**
 * @defgroup nomp_update_direction Update direction
 * Defines the direction in `nomp_update`.
 */

/**
 * @ingroup nomp_update_direction
 * @brief NOMP allocation operation.
 */
#define NOMP_ALLOC 1
/**
 * @ingroup nomp_update_direction
 * @brief Mapping of host to device(H2D) operation.
 */
#define NOMP_TO 2
/**
 * @ingroup nomp_update_direction
 * @brief Mapping of device to host(D2H) operation.
 */
#define NOMP_FROM 4
/**
 * @ingroup nomp_update_direction
 * @brief NOMP freeing operation.
 */
#define NOMP_FREE 8

/**
 * @defgroup nomp_types Types
 * Defines argument type: integer, float or a pointer.
 */

/**
 * @ingroup nomp_types
 * @brief Integer argument type
 */
#define NOMP_INTEGER 1
/**
 * @ingroup nomp_types
 * @brief Float argument type
 */
#define NOMP_FLOAT 2
/**
 * @ingroup nomp_types
 * @brief Pointer argument type
 */
#define NOMP_PTR 4

/**
 * @defgroup nomp_errors Errors
 * Error ids returned by nomp api calls.
 */

/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP backend
 */
#define NOMP_INVALID_BACKEND -32
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP platform
 */
#define NOMP_INVALID_PLATFORM -33
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP device
 */
#define NOMP_INVALID_DEVICE -34
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP map pointer
 */
#define NOMP_INVALID_MAP_PTR -36
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP map operation
 */
#define NOMP_INVALID_MAP_OP -37
/**
 * @ingroup nomp_errors
 * @brief Pointer is already mapped
 */
#define NOMP_PTR_ALREADY_MAPPED -38
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP kernel
 */
#define NOMP_INVALID_KNL -39
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP for clause
 */
#define NOMP_INVALID_CLAUSE -40
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP map parameters
 */
#define NOMP_INVALID_MAP_PARAMS -41

/**
 * @ingroup nomp_errors
 * @brief NOMP is already initialized
 */
#define NOMP_INITIALIZED_ERROR -64
/**
 * @ingroup nomp_errors
 * @brief NOMP is not initialized
 */
#define NOMP_NOT_INITIALIZED_ERROR -65
/**
 * @ingroup nomp_errors
 * @brief Failed to finalize NOMP
 */
#define NOMP_FINALIZE_ERROR -66
/**
 * @ingroup nomp_errors
 * @brief NOMP malloc error
 */
#define NOMP_MALLOC_ERROR -67

/**
 * @ingroup nomp_errors
 * @brief NOMP python initialization failed
 */
#define NOMP_PY_INITIALIZE_ERROR -96
/**
 * @ingroup nomp_errors
 * @brief NOMP_INSTALL_DIR env. variable is not set
 */
#define NOMP_INSTALL_DIR_NOT_FOUND -97
/**
 * @ingroup nomp_errors
 * @brief Specified user callback function not found
 */
#define NOMP_USER_CALLBACK_NOT_FOUND -98
/**
 * @ingroup nomp_errors
 * @brief User callback function failed
 */
#define NOMP_USER_CALLBACK_FAILURE -99
/**
 * @ingroup nomp_errors
 * @brief Loopy conversion failed
 */
#define NOMP_LOOPY_CONVERSION_ERROR -100
/**
 * @ingroup nomp_errors
 * @brief Failed to find loopy kernel
 */
#define NOMP_LOOPY_KNL_NAME_NOT_FOUND -101
/**
 * @ingroup nomp_errors
 * @brief Code generation from loopy kernel failed
 */
#define NOMP_LOOPY_CODEGEN_FAILED -102
/**
 * @ingroup nomp_errors
 * @brief Code generation from loopy kernel failed
 */
#define NOMP_GET_GRIDSIZE_FAILED -103
/**
 * @ingroup nomp_errors
 * @brief Grid size calculation failed
 */
#define NOMP_EVAL_GRIDSIZE_FAILED -103
/**
 * @ingroup nomp_errors
 * @brief NOMP python initialization failed
 */
#define NOMP_PY_APPEND_PATH_ERROR -104

/**
 * @ingroup nomp_errors
 * @brief NOMP kernel build failed
 */
#define NOMP_KNL_BUILD_ERROR -128
/**
 * @ingroup nomp_errors
 * @brief Invalid NOMP kernel argument type
 */
#define NOMP_KNL_ARG_TYPE_ERROR -129
/**
 * @ingroup nomp_errors
 * @brief Setting NOMP kernel argument failed
 */
#define NOMP_KNL_ARG_SET_ERROR -130
/**
 * @ingroup nomp_errors
 * @brief NOMP kernel run failed
 */
#define NOMP_KNL_RUN_ERROR -131

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nomp_user_api API
 */

/**
 * @ingroup nomp_user_api
 * @brief Initializes libnomp with the specified backend, platform and device.
 *
 * @details Initializes the nomp code generation for the specified backend
 * (e.g., Cuda, OpenCL, etc) using the given platform id and device id. Returns
 * a negative value if an error occurs during the initialization, otherwise
 * returns 0. Calling this method twice (without nomp_finalize in between) will
 * return an error (but not segfault) as well. Currently only supports Cuda and
 * OpenCL backends.
 *
 * @param[in] backend Target backend for code generation ("Cuda", "OpenCL",
 * etc.).
 * @param[in] platform Target platform id to share resources and execute kernels
 *                 in the targeted device (only used for OpenCL backend).
 * @param[in] device Target device id to execute kernels.
 * @return int
 *
 * <b>Example usage:</b>
 * @code{.c}
 * int err = nomp_init("OpenCL", 0, 0);
 * @endcode
 */
int nomp_init(const char *backend, int platform, int device);

/**
 * @ingroup nomp_user_api
 * @brief Performs device to host (D2H) and host to device (H2D) memory
 * transfers, allocation and freeing of memory on the device.
 *
 * @param[in] ptr Pointer to the host vector.
 * @param[in] start_idx Start index in the vector.
 * @param[in] end_idx End index in the vector.
 * @param[in] unit_size Size of a single vector element.
 * @param[in] op Operation to perform (One of @ref nomp_update_direction).
 * @return int
 *
 * @details Operation op will be performed on the array slice [start_idx,
 * end_idx), i.e., on array elements start_idx, ... end_idx - 1. This method
 * returns a non-zero value if there is an error and 0 otherwise.
 *
 * <b>Example usage:</b>
 * @code{.c}
 * const int N = 10;
 * double a[10], b[10];
 * for (unsigned i = 0; i < N; i++) {
 *   a[i] = i;
 *   b[i] = N - i;
 * }
 * int err = nomp_update(a, 0, N, sizeof(double), NOMP_TO);
 * int err = nomp_update(b, 0, N, sizeof(double), NOMP_TO);
 * // Code that change array values on the device (e.g., execution of a kernel)
 * int err = nomp_update(a, 0, N, sizeof(double), NOMP_FROM);
 * int err = nomp_update(b, 0, N, sizeof(double), NOMP_FROM);
 * int err = nomp_update(a, 0, N, sizeof(double), NOMP_FREE);
 * int err = nomp_update(b, 0, N, sizeof(double), NOMP_FREE);
 * @endcode
 */
int nomp_update(void *ptr, size_t start_idx, size_t end_idx, size_t unit_size,
                int op);

/**
 * @ingroup nomp_user_api
 * @brief Generate a kernel for the target backend (OpenCL, Cuda, etc.) from C
 * source.
 *
 * @details User defined code transformations will be applied based on the
 * annotations passed to this function. Kernel specific transformations can be
 * specified in the callback function.
 *
 * <b>Example usage:</b>
 * @code{.c}
 * int N = 10;
 * double a[10], b[10];
 * for (unsigned i = 0; i < N; i++) {
 *   a[i] = i;
 *   b[i] = 10 -i
 * }
 * const char *knl = "for (unsigned i = 0; i < N; i++) a[i] += b[i];"
 * static int id = -1;
 * const char *annotations[1] = {0},
 *            *clauses[3] = {"transform", "file:function", 0};
 * int err = nomp_jit(&id, knl, annotations, clauses);
 * @endcode
 *
 * @param[out] id Id of the generated kernel.
 * @param[in] c_src Kernel source in C.
 * @param[in] annotations Annotations to perform user defined transformations.
 * @param[in] clauses Clauses to provide meta information about the kernel.
 * @return int
 */
int nomp_jit(int *id, const char *c_src, const char **annotations,
             const char **clauses);

/**
 * @ingroup nomp_user_api
 * @brief Runs the kernel.
 *
 * @details Runs the kernel with a given kernel id. Kernel id is followed by the
 * number of arguments. Then for each argument we pass the argument type (@ref
 * nomp_types), size of the base type, etc.
 *
 * @param[in] id Id of the kernel to be run.
 * @param[in] nargs Number of arguments to the kernel.
 * @param[in] ... For each variable, variable name, type, sizeof base type, and
 * a pointer to the variable.
 *
 * @return int
 */
int nomp_run(int id, int nargs, ...);

int nomp_err(char *buf, int err, size_t buf_size);

/**
 * @ingroup nomp_user_api
 * @brief Finalizes libnomp runtime.
 *
 * @details Frees allocated runtime resources for libnomp. Returns a non-zero
 * value if an error occurs during the finalize process, otherwise returns 0.
 * Calling this method before `nomp_init` will retrun an error. Calling this
 * method twice will also return an error.
 *
 * @return int
 */
int nomp_finalize(void);

void nomp_chk_(int err, const char *file, unsigned line);
#define nomp_chk(err) nomp_chk_(err, __FILE__, __LINE__)

void nomp_assert_(int cond, const char *file, unsigned line);
#define nomp_assert(cond) nomp_assert_(cond, __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif // _LIB_NOMP_H_
