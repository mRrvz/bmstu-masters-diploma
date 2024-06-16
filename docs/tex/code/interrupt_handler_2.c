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
		__vcpu_write_sys_reg(vcpu, *vcpu_pc(vcpu), ELR_EL1);
		break;
	
	case PSR_MODE_EL2h:
		vbar = __vcpu_read_sys_reg(vcpu, VBAR_EL2);
		sctlr = __vcpu_read_sys_reg(vcpu, SCTLR_EL2);
		__vcpu_write_sys_reg(vcpu, *vcpu_pc(vcpu), ELR_EL2);
		break;
	default:
		BUG();
	}

	*vcpu_pc(vcpu) = vbar + exc_offset + type;

	old = *vcpu_cpsr(vcpu);

	if (need_intercepted(old)) {
		new = do_optee_handle_action(target_mode, type, old);
