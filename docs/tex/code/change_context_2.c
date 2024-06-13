int secchg_change_context(vm_id_t vm_id, unsigned flags)
{
	struct secchg_vm_map current =
		secchg_get_current_vm_map();
	struct secchg_vm_map swap_to =
		secchg_get_vm_partition(vm_id);
	int ret;

	if (secchg_validate_vm_map(swap_to)) {
		DMSG("failed to change context to %d", vm_id);
		return -EINVAL;
	}

	ret = __save_vm_map(current);
	if (ret)
		return ret;

	__save_current_context();

	return __swap_to_next_vm(vm_id, flags);
}
