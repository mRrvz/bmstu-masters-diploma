static inline int __sec_vm_pgtable_visit(
	struct sec_vm_pgtable_walk_data *data,
	struct sec_vm_pgtable_mm_ops *mm_ops,
	sec_vm_pteref_t pteref, s8 level)
{
	enum sec_vm_pgtable_walk_flags flags = data->walker->flags;
	sec_vm_pte_t *ptep = data->walker->ptep;
	struct sec_vm_pgtable_visit_ctx ctx = {
		.ptep	= ptep,
		.old	= READ_ONCE(*ptep),
		.arg	= data->walker->arg,
		.mm_ops	= mm_ops,
		.start	= data->start,
		.addr	= data->addr,
		.end	= data->end,
		.level	= level,
		.flags	= flags,
	};
	int ret = 0;
	bool reload = false;
	sec_vm_pteref_t childp;
	bool table = sec_vm_pte_table(ctx.old, level);

	if (table && (ctx.flags & sec_vm_PGTABLE_WALK_TABLE_PRE)) {
		ret = sec_vm_pgtable_visitor_cb(data, &ctx,
					sec_vm_PGTABLE_WALK_TABLE_PRE);
		reload = true;
	}

	if (!table && (ctx.flags & sec_vm_PGTABLE_WALK_LEAF)) {
		ret = sec_vm_pgtable_visitor_cb(data, &ctx,
					sec_vm_PGTABLE_WALK_LEAF);
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
				    sec_vm_PGTABLE_WALK_TABLE_POST);

out:
	if (sec_vm_pgtable_walk_continue(data->walker, ret))
		return 0;

	return ret;
}

static int sec_vm_pgtable_walk(
	struct sec_vm_pgtable_walk_data *data,
	struct sec_vm_pgtable_mm_ops *mm_ops,
	sec_vm_pteref_t pgtable, s8 level)
{
	u32 idx;
	int ret = 0;

	if (level < sec_vm_PGTABLE_FIRST_LEVEL ||
			 level > sec_vm_PGTABLE_LAST_LEVEL)
		return -EINVAL;

	for (idx = sec_vm_pgtable_idx(data, level);
		idx < PTRS_PER_PTE; ++idx) {
		sec_vm_pteref_t pteref = &pgtable[idx];

		if (data->addr >= data->end)
			break;

		ret = __sec_vm_pgtable_visit(data, mm_ops, 
					     pteref, level);
		if (ret)
			break;
	}

	return ret;
}
