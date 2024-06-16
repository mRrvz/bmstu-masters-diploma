static inline int __sec_vm_pgtable_visit(
	struct sec_vm_pgtable_walk_data *data,
	struct sec_vm_pgtable_mm_ops *mm_ops,
	sec_vm_pteref_t pteref, s8 level)
{
	enum sec_vm_pgtable_walk_flags flags = data->flags;
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

	if (table && (ctx.flags & SEC_VM_PGTABLE_TABLE_PRE)) {
		ret = sec_vm_pgtable_visitor_cb(data, &ctx,
					SEC_VM_PGTABLE_WALK_TABLE_PRE);
		reload = true;
	}

	if (!table && (ctx.flags & SEC_VM_PGTABLE_WALK_LEAF)) {
		ret = sec_vm_pgtable_visitor_cb(data, &ctx,
					SEC_VM_PGTABLE_WALK_LEAF);
