```
================================================================================
Running task system grading harness... (22 total tests)
  - Detected CPU with 4 execution contexts
  - Task system configured to use at most 8 threads
================================================================================
================================================================================
Executing test: super_super_light...
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                8.338     8.389       0.99  (OK)
[Parallel + Always Spawn]               46.094    54.383      0.85  (OK)
[Parallel + Thread Pool + Spin]         17.055    47.597      0.36  (OK)
[Parallel + Thread Pool + Sleep]        19.059    12.608      1.51  (NOT OK)
================================================================================
Executing test: super_super_light_async...
Results for: super_super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                8.354     8.404       0.99  (OK)
[Parallel + Always Spawn]               13.999    55.395      0.25  (OK)
[Parallel + Thread Pool + Spin]         13.677    26.666      0.51  (OK)
[Parallel + Thread Pool + Sleep]        13.979    10.657      1.31  (NOT OK)
================================================================================
Executing test: super_light...
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                89.307    116.398     0.77  (OK)
[Parallel + Always Spawn]               96.013    101.305     0.95  (OK)
[Parallel + Thread Pool + Spin]         64.269    109.494     0.59  (OK)
[Parallel + Thread Pool + Sleep]        63.936    67.048      0.95  (OK)
================================================================================
Executing test: super_light_async...
Results for: super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                89.276    117.207     0.76  (OK)
[Parallel + Always Spawn]               71.112    100.79      0.71  (OK)
[Parallel + Thread Pool + Spin]         67.452    80.345      0.84  (OK)
[Parallel + Thread Pool + Sleep]        69.162    74.506      0.93  (OK)
================================================================================
Executing test: ping_pong_equal...
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1442.407  1877.638    0.77  (OK)
[Parallel + Always Spawn]               910.883   1028.092    0.89  (OK)
[Parallel + Thread Pool + Spin]         840.524   1236.856    0.68  (OK)
[Parallel + Thread Pool + Sleep]        839.26    991.787     0.85  (OK)
================================================================================
Executing test: ping_pong_equal_async...
Results for: ping_pong_equal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1451.004  1881.116    0.77  (OK)
[Parallel + Always Spawn]               837.339   1029.782    0.81  (OK)
[Parallel + Thread Pool + Spin]         832.986   1252.628    0.66  (OK)
[Parallel + Thread Pool + Sleep]        833.146   1025.67     0.81  (OK)
================================================================================
Executing test: ping_pong_unequal...
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                2532.003  2452.024    1.03  (OK)
[Parallel + Always Spawn]               1333.656  1305.536    1.02  (OK)
[Parallel + Thread Pool + Spin]         1301.364  1636.859    0.80  (OK)
[Parallel + Thread Pool + Sleep]        1297.269  1277.869    1.02  (OK)
================================================================================
Executing test: ping_pong_unequal_async...
Results for: ping_pong_unequal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                2535.181  2456.022    1.03  (OK)
[Parallel + Always Spawn]               1292.255  1305.478    0.99  (OK)
[Parallel + Thread Pool + Spin]         1287.762  1625.302    0.79  (OK)
[Parallel + Thread Pool + Sleep]        1286.702  1302.037    0.99  (OK)
================================================================================
Executing test: recursive_fibonacci...
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1284.658  1392.133    0.92  (OK)
[Parallel + Always Spawn]               663.422   679.983     0.98  (OK)
[Parallel + Thread Pool + Spin]         662.333   801.293     0.83  (OK)
[Parallel + Thread Pool + Sleep]        663.667   684.598     0.97  (OK)
================================================================================
Executing test: recursive_fibonacci_async...
Results for: recursive_fibonacci_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1284.809  1391.144    0.92  (OK)
[Parallel + Always Spawn]               646.895   679.92      0.95  (OK)
[Parallel + Thread Pool + Spin]         646.897   738.808     0.88  (OK)
[Parallel + Thread Pool + Sleep]        647.797   679.018     0.95  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1850.071  1858.966    1.00  (OK)
[Parallel + Always Spawn]               1389.166  1030.783    1.35  (NOT OK)
[Parallel + Thread Pool + Spin]         1043.315  1436.264    0.73  (OK)
[Parallel + Thread Pool + Sleep]        1040.169  827.997     1.26  (NOT OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_async...
Results for: math_operations_in_tight_for_loop_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1857.827  1857.36     1.00  (OK)
[Parallel + Always Spawn]               1038.932  1029.518    1.01  (OK)
[Parallel + Thread Pool + Spin]         1037.371  1145.084    0.91  (OK)
[Parallel + Thread Pool + Sleep]        1039.098  1198.745    0.87  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1847.826  1858.804    0.99  (OK)
[Parallel + Always Spawn]               1285.502  1037.551    1.24  (NOT OK)
[Parallel + Thread Pool + Spin]         1137.28   1458.775    0.78  (OK)
[Parallel + Thread Pool + Sleep]        1133.542  969.813     1.17  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks_async...
Results for: math_operations_in_tight_for_loop_fewer_tasks_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1849.209  1859.226    0.99  (OK)
[Parallel + Always Spawn]               771.113   1037.105    0.74  (OK)
[Parallel + Thread Pool + Spin]         774.459   860.592     0.90  (OK)
[Parallel + Thread Pool + Sleep]        771.884   764.977     1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                949.893   952.25      1.00  (OK)
[Parallel + Always Spawn]               532.024   431.374     1.23  (NOT OK)
[Parallel + Thread Pool + Spin]         459.57    585.486     0.78  (OK)
[Parallel + Thread Pool + Sleep]        460.577   415.873     1.11  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in_async...
Results for: math_operations_in_tight_for_loop_fan_in_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                947.505   953.627     0.99  (OK)
[Parallel + Always Spawn]               394.958   430.167     0.92  (OK)
[Parallel + Thread Pool + Spin]         396.159   436.493     0.91  (OK)
[Parallel + Thread Pool + Sleep]        395.85    393.66      1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                946.524   950.761     1.00  (OK)
[Parallel + Always Spawn]               404.882   400.539     1.01  (OK)
[Parallel + Thread Pool + Spin]         424.312   562.105     0.75  (OK)
[Parallel + Thread Pool + Sleep]        420.446   396.951     1.06  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree_async...
Results for: math_operations_in_tight_for_loop_reduction_tree_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                947.12    953.824     0.99  (OK)
[Parallel + Always Spawn]               391.929   400.122     0.98  (OK)
[Parallel + Thread Pool + Spin]         395.371   438.058     0.90  (OK)
[Parallel + Thread Pool + Sleep]        396.776   390.173     1.02  (OK)
================================================================================
Executing test: spin_between_run_calls...
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                456.451   478.421     0.95  (OK)
[Parallel + Always Spawn]               235.553   246.914     0.95  (OK)
[Parallel + Thread Pool + Spin]         235.309   636.797     0.37  (OK)
[Parallel + Thread Pool + Sleep]        235.402   247.102     0.95  (OK)
================================================================================
Executing test: spin_between_run_calls_async...
Results for: spin_between_run_calls_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                456.361   478.467     0.95  (OK)
[Parallel + Always Spawn]               235.459   246.917     0.95  (OK)
[Parallel + Thread Pool + Spin]         235.194   601.78      0.39  (OK)
[Parallel + Thread Pool + Sleep]        235.389   247.021     0.95  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                487.579   490.051     0.99  (OK)
[Parallel + Always Spawn]               149.528   146.324     1.02  (OK)
[Parallel + Thread Pool + Spin]         151.792   166.389     0.91  (OK)
[Parallel + Thread Pool + Sleep]        151.723   147.163     1.03  (OK)
================================================================================
Executing test: mandelbrot_chunked_async...
Results for: mandelbrot_chunked_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                487.735   490.634     0.99  (OK)
[Parallel + Always Spawn]               148.698   147.058     1.01  (OK)
[Parallel + Thread Pool + Spin]         151.208   168.821     0.90  (OK)
[Parallel + Thread Pool + Sleep]        148.716   147.52      1.01  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : Perf did not pass all tests
[Parallel + Thread Pool + Spin]         : All passed Perf
[Parallel + Thread Pool + Sleep]        : Perf did not pass all tests

```