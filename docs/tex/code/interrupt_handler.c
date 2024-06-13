static void enter_exception64(struct kvm_vcpu *vcpu,
			      unsigned long target_mode,
			      enum exception_type type)
{
	unsigned long sctlr, vbar, old, new, mode;
	u64 exc_offset;

	mode = *vcpu_cpsr(vcpu) & (PSR_MODE_MASK | PSR_MODE32_BIT);

	if (mode == target_mode)
		exc_offset = CURRENT_EL_SP_ELx_VECTOR;
	else if ((mode | PSR_MODE_THREAD_BIT) == target_mode)
		exc_offset = CURRENT_EL_SP_EL0_VECTOR;
	else if (!(mode & PSR_MODE32_BIT))
		exc_offset = LOWER_EL_AArch64_VECTOR;
	else
		exc_offset = LOWER_EL_AArch32_VECTOR;

	switch (target_mode) {
	case PSR_MODE_EL1h:
		vbar = __vcpu_read_sys_reg(vcpu, VBAR_EL1);
		sctlr = __vcpu_read_sys_reg(vcpu, SCTLR_EL1);
