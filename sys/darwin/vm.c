#include <sys/compiler.h>
#include <mach/mach_traps.h>
#include <mem/vm.h>

int
vm_usage(struct vm_info *vi)
{
	mach_port_t self = mach_task_self();
	struct mach_task_basic_info v;
	mach_msg_type_number_t msg = MACH_TASK_BASIC_INFO_COUNT;

	memset(vi, 0, sizeof(*vi));
	memset(&v, 0, sizeof(v));

	int rv = task_info(self, MACH_TASK_BASIC_INFO, (task_info_t)&v, &msg);
	vi->bla = (u64)v.resident_size;

	return rv == KERN_SUCCESS ? 0 : rv;
}
