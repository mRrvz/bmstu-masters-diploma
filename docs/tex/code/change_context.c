static int __swap_to_next_vm(vm_id_t vm_id, unsigned flags)
{
	struct secchg_partition *prtn;
	uint32_t exceptions;

	prtn = get_current_prtn();

	if (prtn && prtn->id == vm_id)
		return 0;

	exceptions = cpu_spin_lock_xsave(&prtn_list_lock);
	LIST_FOREACH(prtn, &prtn_list, link) {
		if (prtn->id == guest_id) {
			set_current_prtn(prtn, flags);
			core_mmu_set_prtn(prtn->mmu_prtn, flags);
			refcount_inc(&prtn->refc);
			cpu_spin_unlock_xrestore(&prtn_list_lock,
						 exceptions);
			return 0;
		}
	}
	cpu_spin_unlock_xrestore(&prtn_list_lock, exceptions);

	return 0;
}

int secchg_change_context(vm_id_t vm_id, unsigned flags)
{
	struct secchg_vm_map current =
		secchg_get_current_vm_map();
