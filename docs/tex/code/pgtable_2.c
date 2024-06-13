	if (!table && (ctx.flags & SEC_VM_PGTABLE_WALK_LEAF)) {
		ret = sec_vm_pgtable_visitor_cb(data, &ctx,
					SEC_VM_PGTABLE_WALK_LEAF);
		reload = true;
	}

	if (reload) {
		ctx.old = READ_ONCE(*ptep);
		table = sec_vm_pte_table(ctx.old, level);
	}

	if (!sec_vm_pgtable_walk_continue(data->walker, ret))
		goto out;

	if (!table) {
		data->addr = ALIGN_DOWN(data->addr, 
					sec_vm_granule_size(level));
		data->addr += sec_vm_granule_size(level);
		goto out;
	}

	childp = (sec_vm_pteref_t)
	    sec_vm_pte_follow(ctx.old, mm_ops);
	ret = __sec_vm_pgtable_walk(data, mm_ops, childp,
				    level + 1);
	if (!sec_vm_pgtable_walk_continue(data->walker, ret))
		goto out;

	if (ctx.flags & sec_vm_PGTABLE_WALK_TABLE_POST)
		ret = sec_vm_pgtable_visitor_cb(data, &ctx,
				    SEC_VM_PGTABLE_WALK_TABLE_POST);
