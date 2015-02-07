# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


# Makefile for the different targets used to generate full packages of a kernel
# It uses the generic clean infrastructure of kbuild

# RPM target
# ---------------------------------------------------------------------------
# The rpm target generates two rpm files:
# /usr/src/packages/SRPMS/kernel-2.6.7rc2-1.src.rpm
# /usr/src/packages/RPMS/i386/kernel-2.6.7rc2-1.<arch>.rpm
# The src.rpm files includes all source for the kernel being built
# The <arch>.rpm includes kernel configuration, modules etc.
#
# Process to create the rpm files
# a) clean the kernel
# b) Generate .spec file
# c) Build a tar ball, using symlink to make kernel version
#    first entry in the path
# d) and pack the result to a tar.gz file
# e) generate the rpm files, based on kernel.spec
# - Use /. to avoid tar packing just the symlink

# Do we have rpmbuild, otherwise fall back to the older rpm
RPM := $(shell if [ -x "/usr/bin/rpmbuild" ]; then echo rpmbuild; \
	           else echo rpm; fi)

# Remove hyphens since they have special meaning in RPM filenames
KERNELPATH := kernel-$(subst -,,$(KERNELRELEASE))
MKSPEC     := $(srctree)/scripts/package/mkspec
PREV       := set -e; cd ..;

# rpm-pkg
# ---------------------------------------------------------------------------
$(objtree)/kernel.spec: $(MKSPEC) $(srctree)/Makefile
	$(CONFIG_SHELL) $(MKSPEC) > $@

rpm-pkg rpm: $(objtree)/kernel.spec FORCE
	$(MAKE) clean
	$(PREV) ln -sf $(srctree) $(KERNELPATH)
	$(CONFIG_SHELL) $(srctree)/scripts/setlocalversion > $(objtree)/.scmversion
	$(PREV) tar -cz $(RCS_TAR_IGNORE) -f $(KERNELPATH).tar.gz $(KERNELPATH)/.
	$(PREV) rm $(KERNELPATH)
	rm -f $(objtree)/.scmversion
	set -e; \
	$(CONFIG_SHELL) $(srctree)/scripts/mkversion > $(objtree)/.tmp_version
	set -e; \
	mv -f $(objtree)/.tmp_version $(objtree)/.version

	$(RPM) $(RPMOPTS) --target $(UTS_MACHINE) -ta ../$(KERNELPATH).tar.gz
	rm ../$(KERNELPATH).tar.gz

clean-files := $(objtree)/kernel.spec

# binrpm-pkg
# ---------------------------------------------------------------------------
$(objtree)/binkernel.spec: $(MKSPEC) $(srctree)/Makefile
	$(CONFIG_SHELL) $(MKSPEC) prebuilt > $@

binrpm-pkg: $(objtree)/binkernel.spec FORCE
	$(MAKE) KBUILD_SRC=
	set -e; \
	$(CONFIG_SHELL) $(srctree)/scripts/mkversion > $(objtree)/.tmp_version
	set -e; \
	mv -f $(objtree)/.tmp_version $(objtree)/.version

	$(RPM) $(RPMOPTS) --define "_builddir $(srctree)" --target \
		$(UTS_MACHINE) -bb $<

clean-files += $(objtree)/binkernel.spec

# Deb target
# ---------------------------------------------------------------------------
deb-pkg: FORCE
	$(MAKE) KBUILD_SRC=
	$(CONFIG_SHELL) $(srctree)/scripts/package/builddeb

clean-dirs += $(objtree)/debian/


# tarball targets
# ---------------------------------------------------------------------------
tar%pkg: FORCE
	$(MAKE) KBUILD_SRC=
	$(CONFIG_SHELL) $(srctree)/scripts/package/buildtar $@

clean-dirs += $(objtree)/tar-install/


# Help text displayed when executing 'make help'
# ---------------------------------------------------------------------------
help: FORCE
	@echo '  rpm-pkg         - Build both source and binary RPM kernel packages'
	@echo '  binrpm-pkg      - Build only the binary kernel package'
	@echo '  deb-pkg         - Build the kernel as an deb package'
	@echo '  tar-pkg         - Build the kernel as an uncompressed tarball'
	@echo '  targz-pkg       - Build the kernel as a gzip compressed tarball'
	@echo '  tarbz2-pkg      - Build the kernel as a bzip2 compressed tarball'

