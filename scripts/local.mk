nix_noinst_scripts := \
  $(d)/nix-http-export.cgi \
  $(d)/nix-profile.sh \
  $(d)/nix-profile.fish \
  $(d)/nix-reduce-build

noinst-scripts += $(nix_noinst_scripts)

profiledir = $(sysconfdir)/profile.d
fish_vendorconf_dir = $(datadir)/fish/vendor_conf.d

$(eval $(call install-file-as, $(d)/nix-profile.sh, $(profiledir)/nix.sh, 0644))
$(eval $(call install-file-as, $(d)/nix-profile.fish, $(fish_vendorconf_dir)/nix.fish, 0644))

clean-files += $(nix_noinst_scripts)
