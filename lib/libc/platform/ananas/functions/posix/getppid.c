#include <ananas/types.h>
#include <ananas/syscalls.h>
#include <unistd.h>
#include "_map_statuscode.h"

pid_t
getppid()
{
	statuscode_t status = sys_getppid();
	return map_statuscode(status);
}
