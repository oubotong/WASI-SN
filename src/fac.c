// C program to find factorial of given number 
  
#include "bh_platform.h"
#include "wasm_export.h"
#include "math.h"
  
// Function to find factorial of given number 
int factorial(wasm_exec_env_t exec_env,int n) 
{ 
    if (n == 0) 
        return 1; 
    return n * factorial(exec_env,n - 1); 
} 

