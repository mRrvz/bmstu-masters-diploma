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
	} else {
		new = 0;

		new |= (old & PSR_N_BIT);
		new |= (old & PSR_Z_BIT);
		new |= (old & PSR_C_BIT);
		new |= (old & PSR_V_BIT);

		if (kvm_has_mte(kern_hyp_va(vcpu->kvm)))
			new |= PSR_TCO_BIT;
