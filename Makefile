# Name of the module
LOCAL_NAME := DB9

# List of submodules which contain code we need to include in the final lib
LOCAL_API_DEPENDS := DB9/include\

LOCAL_ADD_INCLUDE := include\
                     include/std_inc \
                     include/api_inc \


# Set this to any non-null string to signal a module which 
# generates a binary 
IS_ENTRY_POINT := no

## ------------------------------------ ##
##     Add your custom flags here          ##
## ------------------------------------ ##
MYCFLAGS += -lm

## ------------------------------------- ##
##    List all your sources here           ##
## ------------------------------------- ##
C_SRC := ${notdir ${wildcard src/*.c}}

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/platform/compilation/cust_rules.mk