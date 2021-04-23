################################################################################
#
# timer
#
################################################################################

TIMER_VERSION = 1.0
TIMER_SITE = $(TOPDIR)/../timer_src
TIMER_SITE_METHOD = local
TIMER_DEPENDENCIES = libgpiod
TIMER_INSTALL_STAGING = YES

define TIMER_BUILD_CMDS
   $(MAKE) $(TARGET_CONFIGURE_OPTS) timer -C $(@D)
endef
define TIMER_INSTALL_TARGET_CMDS 
   $(INSTALL) -D -m 0755 $(@D)/timer $(TARGET_DIR)/usr/bin 
endef
TIMER_LICENSE = Proprietary

$(eval $(generic-package))