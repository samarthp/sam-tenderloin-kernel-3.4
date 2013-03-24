/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <mach/irqs.h>
#include <asm/mach-types.h>
#include <mach/gpiomux.h>
#include "gpiomux-tenderloin.h"

static struct gpiomux_setting console_uart = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

/* The SPI configurations apply to GSBI1 and GSBI10 */
static struct gpiomux_setting spi_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting spi_suspended_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting spi_suspended_cs_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

/* This I2C active configuration applies to GSBI3 and GSBI4 */
static struct gpiomux_setting i2c_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting i2c_active_gsbi7 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

/* This I2C suspended configuration applies to GSBI3, GSBI4 and GSBI7 */
static struct gpiomux_setting i2c_suspended_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi8 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting ps_hold = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_snddev_active_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_snddev_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting ebi2_a_d = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ebi2_oe = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ebi2_we = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ebi2_cs2 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ebi2_cs3 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
static struct gpiomux_setting ebi2_cs4 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
#endif

static struct gpiomux_setting ebi2_adv = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
static struct gpiomux_setting usb_isp1763_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting usb_isp1763_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting usb_isp1763_dack_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_isp1763_dack_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_isp1763_dreq_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting usb_isp1763_dreq_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting usb_isp1763_rst_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_isp1763_rst_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_3g_3v3_en_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting usb_3g_3v3_en_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_3g_disable_n_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_3g_disable_n_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting usb_3g_wake_n_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting usb_3g_wake_n_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting usb_3g_uim_cd_n_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting usb_3g_uim_cd_n_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
#endif

static struct gpiomux_setting wlan_pwdn_n_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wlan_pwdn_n_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdcc1_dat_0_3_cmd_actv_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdcc1_dat_4_7_cmd_actv_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdcc1_clk_actv_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdcc1_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdcc2_dat_0_3_cmd_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdcc2_dat_4_7_cmd_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdcc2_clk_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdcc2_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting usb_hub_reset_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting usb_hub_reset_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting usb_switch_cntl_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting usb_switch_cntl_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting usb_switch_en_actv_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting usb_switch_en_susp_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdcc5_dat_0_3_cmd_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdcc5_clk_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdcc5_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting aux_pcm_active_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting aux_pcm_suspend_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting uart1dm_out_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting uart1dm_suspended_out_high = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting uart1dm_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting uart1dm_suspended = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting mi2s_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mi2s_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting lcdc_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_14MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting lcdc_active_cfg_2m = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting lcdc_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdp_vsync_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_status_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdm2ap_status_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting cam_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting cam_suspend_3_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting cam_suspend_5_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting mdm2ap_sync_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdm2ap_sync_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting tm_active = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting tm_suspended = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting tma_active = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ts_suspended = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdp_vsync_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting hdmi_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting hdmi_active_2_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting hdmi_active_3_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting pmic_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting pmic_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting cam_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting cam_active_2_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting cam_active_3_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting cam_active_4_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting cam_active_5_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

#ifdef CONFIG_MSM_GSBI9_UART
static struct gpiomux_setting uart9dm_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA ,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi9 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct gpiomux_setting ap2mdm_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_status_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdm2ap_vfr_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting mdm2ap_vfr_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_errfatal_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting ap2mdm_kpdpwr_n_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};


static struct gpiomux_setting mdm2ap_vddmin_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdm2ap_vddmin_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm8x60_gsbi_configs[] __initdata = {
#if 0
	{
		.gpio      = 33,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
	{
		.gpio      = 34,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
	{
		.gpio      = 35,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_cs_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
	{
		.gpio      = 36,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
#endif
	{
		.gpio      = 43,
		.settings = {
			[GPIOMUX_SUSPENDED] = &i2c_suspended_config,
			[GPIOMUX_ACTIVE]    = &i2c_active,
		},
	},
	{
		.gpio      = 44,
		.settings = {
			[GPIOMUX_SUSPENDED] = &i2c_suspended_config,
			[GPIOMUX_ACTIVE]    = &i2c_active,
		},
	},
#if 0
	{
		.gpio      = 47,
		.settings = {
			[GPIOMUX_SUSPENDED] = &i2c_suspended_config,
			[GPIOMUX_ACTIVE]    = &i2c_active,
		},
	},
	{
		.gpio      = 48,
		.settings = {
			[GPIOMUX_SUSPENDED] = &i2c_suspended_config,
			[GPIOMUX_ACTIVE]    = &i2c_active,
		},
	},
	{
		.gpio      = 59,
		.settings = {
			[GPIOMUX_SUSPENDED] = &i2c_suspended_config,
			[GPIOMUX_ACTIVE]    = &i2c_active_gsbi7,
		},
	},
	{
		.gpio      = 60,
		.settings = {
			[GPIOMUX_SUSPENDED] = &i2c_suspended_config,
			[GPIOMUX_ACTIVE]    = &i2c_active_gsbi7,
		},
	},
	{
		.gpio      = 64,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi8,
		},
	},
	{
		.gpio      = 65,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi8,
		},
	},
#endif
};

static struct msm_gpiomux_config msm8x60_fluid_gsbi_configs[] __initdata = {
	{
		.gpio      = 70,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
	{
		.gpio      = 72,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_cs_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
	{
		.gpio      = 73,
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE]    = &spi_active,
		},
	},
};

static struct msm_gpiomux_config msm8x60_ebi2_configs[] __initdata = {
	{
		.gpio      = 40,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_cs2,
		},
	},
	{
		.gpio      = 92,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ps_hold,
		},
	},
	{
		.gpio      = 123,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 124,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 125,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 127,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 128,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 129,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 130,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
	/* ISP VDD_3V3_EN */
	{
		.gpio      = 132,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_cs4,
		},
	},
#endif
	{
		.gpio      = 133,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_cs3,
		},
	},
	{
		.gpio      = 135,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 136,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 137,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 138,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 139,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 140,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 141,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 142,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 143,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 144,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 145,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 146,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 147,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 148,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 149,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 150,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_a_d,
		},
	},
	{
		.gpio      = 151,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_oe,
		},
	},
	{
		.gpio      = 153,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_adv,
		},
	},
	{
		.gpio      = 157,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ebi2_we,
		},
	},
};

#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
static struct msm_gpiomux_config msm8x60_isp_usb_configs[] __initdata = {
	{
		.gpio      = ISP1763_INT_GPIO,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_isp1763_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_isp1763_susp_cfg,
		},
	},
	{
		.gpio      = ISP1763_DACK_GPIO,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_isp1763_dack_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_isp1763_dack_susp_cfg,
		},
	},
	{
		.gpio      = ISP1763_DREQ_GPIO,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_isp1763_dreq_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_isp1763_dreq_susp_cfg,
		},
	},
	{
		.gpio      = ISP1763_RST_GPIO,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_isp1763_rst_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_isp1763_rst_susp_cfg,
		},
	},
	{
		.gpio      = GPIO_3G_3V3_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_3g_3v3_en_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_3g_3v3_en_susp_cfg,
		},
	},
	{
		.gpio      = GPIO_3G_DISABLE_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_3g_disable_n_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_3g_disable_n_susp_cfg,
		},
	},
	{
		.gpio      = GPIO_3G_WAKE_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_3g_wake_n_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_3g_wake_n_susp_cfg,
		},
	},
	{
		.gpio      = GPIO_3G_UIM_CD_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &usb_3g_uim_cd_n_actv_cfg,
			[GPIOMUX_SUSPENDED] = &usb_3g_uim_cd_n_susp_cfg,
		},
	},
};
#endif

static struct msm_gpiomux_config msm8x60_uart_configs[] __initdata = {
	{ /* UARTDM_TX */
		.gpio      = 53,
		.settings = {
			[GPIOMUX_ACTIVE]    = &uart1dm_out_active,
			[GPIOMUX_SUSPENDED] = &uart1dm_suspended_out_high,
		},
	},
	{ /* UARTDM_RX */
		.gpio      = 54,
		.settings = {
			[GPIOMUX_ACTIVE]    = &uart1dm_active,
			[GPIOMUX_SUSPENDED] = &uart1dm_suspended,
		},
	},
	{ /* UARTDM_CTS */
		.gpio      = 55,
		.settings = {
			[GPIOMUX_ACTIVE]    = &uart1dm_active,
			[GPIOMUX_SUSPENDED] = &uart1dm_suspended,
		},
	},
	{ /* UARTDM_RFR */
		.gpio      = 56,
		.settings = {
			[GPIOMUX_ACTIVE]    = &uart1dm_out_active,
			[GPIOMUX_SUSPENDED] = &uart1dm_suspended_out_high,
		},
	},
	{ /* DEBUG_RX */
		.gpio      = 117,
		.settings = {
			[GPIOMUX_SUSPENDED] = &console_uart,
		},
	},
	{ /* DEBUG_TX */
		.gpio      = 118,
		.settings = {
			[GPIOMUX_SUSPENDED] = &console_uart,
		},
	},
};

#ifdef CONFIG_MSM_GSBI9_UART
static struct msm_gpiomux_config msm8x60_charm_uart_configs[] __initdata = {
	{ /* UART9DM  RX */
		.gpio	   = 66,
		.settings = {
			[GPIOMUX_ACTIVE]    = &uart9dm_active,
			[GPIOMUX_SUSPENDED] = &gsbi9,
		},
	},
	{ /* UART9DM TX */
		.gpio	   = 67,
		.settings = {
			[GPIOMUX_ACTIVE]    = &uart9dm_active,
			[GPIOMUX_SUSPENDED] = &gsbi9,
		},
	},
};
#endif

static struct msm_gpiomux_config msm8x60_ts_configs[] __initdata = {
	{
		/* TS_ATTN */
		.gpio = 58,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ts_suspended,
		},
	},
};

static struct msm_gpiomux_config msm8x60_tmg200_configs[] __initdata = {
	{
		.gpio = 61,
		.settings = {
			[GPIOMUX_ACTIVE] = &tm_active,
			[GPIOMUX_SUSPENDED] = &tm_suspended,
		},
	},
};

static struct msm_gpiomux_config msm8x60_tma300_configs[] __initdata = {
	{
		.gpio = 61,
		.settings = {
			[GPIOMUX_ACTIVE]    = &tma_active,
			[GPIOMUX_SUSPENDED] = &tm_suspended,
		},
	},
};

static struct msm_gpiomux_config msm8x60_aux_pcm_configs[] __initdata = {
	{
		.gpio = 111,
		.settings = {
			[GPIOMUX_ACTIVE]    = &aux_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &aux_pcm_suspend_config,
		},
	},
	{
		.gpio = 112,
		.settings = {
			[GPIOMUX_ACTIVE]    = &aux_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &aux_pcm_suspend_config,
		},
	},
	{
		.gpio = 113,
		.settings = {
			[GPIOMUX_ACTIVE]    = &aux_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &aux_pcm_suspend_config,
		},
	},
	{
		.gpio = 114,
		.settings = {
			[GPIOMUX_ACTIVE]    = &aux_pcm_active_config,
			[GPIOMUX_SUSPENDED] = &aux_pcm_suspend_config,
		},
	},
};

static struct msm_gpiomux_config msm8x60_sdc_configs[] __initdata = {
	/* SDCC1 data[0] */
	{
		.gpio = 159,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[1] */
	{
		.gpio = 160,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[2] */
	{
		.gpio = 161,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[3] */
	{
		.gpio = 162,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[4] */
	{
		.gpio = 163,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[5] */
	{
		.gpio = 164,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[6] */
	{
		.gpio = 165,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 data[7] */
	{
		.gpio = 166,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 CLK */
	{
		.gpio = 167,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_clk_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
	/* SDCC1 CMD */
	{
		.gpio = 168,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc1_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc1_suspend_config,
		},
	},
};

/* SAM: Added from 2.6.35 */
static struct msm_gpiomux_config msm_qrdc_sdc_configs[] __initdata = {
    {
        .gpio      = 118,
        .settings = {
            [GPIOMUX_ACTIVE]    = &wlan_pwdn_n_actv_cfg,
            [GPIOMUX_SUSPENDED] = &wlan_pwdn_n_susp_cfg,
        },
    },
};

static struct msm_gpiomux_config msm8x60_charm_sdc_configs[] __initdata = {
	/* SDCC5 cmd */
	{
		.gpio = 95,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc5_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc5_suspend_config,
		},
	},
	/* SDCC5 data[3]*/
	{
		.gpio = 96,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc5_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc5_suspend_config,
		},
	},
	/* SDCC5 clk */
	{
		.gpio = 97,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc5_clk_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc5_suspend_config,
		},
	},
	/* SDCC5 data[2]*/
	{
		.gpio = 98,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc5_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc5_suspend_config,
		},
	},
	/* SDCC5 data[1]*/
	{
		.gpio = 99,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc5_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc5_suspend_config,
		},
	},
	/* SDCC5 data[0]*/
	{
		.gpio = 100,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc5_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc5_suspend_config,
		},
	},
	/* MDM2AP_SYNC */
	{
		.gpio = 129,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mdm2ap_sync_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdm2ap_sync_suspend_cfg,
		},
	},
#if 0
	/* MDM2AP_VDDMIN */
	{
		.gpio = 140,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mdm2ap_vddmin_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdm2ap_vddmin_suspend_cfg,
		},
	},
#endif
	/* SDCC2 data[0] */
	{
		.gpio = 143,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[1] */
	{
		.gpio = 144,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[2] */
	{
		.gpio = 145,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[3] */
	{
		.gpio = 146,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[4] */
	{
		.gpio = 147,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[5] */
	{
		.gpio = 148,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[6] */
	{
		.gpio = 149,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 data[7] */
	{
		.gpio = 150,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_4_7_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
	/* SDCC2 CMD */
	{
		.gpio = 151,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_dat_0_3_cmd_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},

	/* SDCC2 CLK */
	{
		.gpio = 152,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdcc2_clk_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdcc2_suspend_config,
		},
	},
};

/* SAM: Added by me from HP Kernel 2.6.35 */
static struct msm_gpiomux_config msm_qrdc_usb_configs[] __initdata = {
    {
        .gpio      = 34,
        .settings = {
            [GPIOMUX_ACTIVE]    = &usb_hub_reset_actv_cfg,
            [GPIOMUX_SUSPENDED] = &usb_hub_reset_susp_cfg,
        },
    },
    {
        .gpio      = 131,
        .settings = {
            [GPIOMUX_ACTIVE]    = &usb_switch_cntl_actv_cfg,
            [GPIOMUX_SUSPENDED] = &usb_switch_cntl_susp_cfg,
        },
    },
    {
        .gpio      = 132,
        .settings = {
            [GPIOMUX_ACTIVE]    = &usb_switch_en_actv_cfg,
            [GPIOMUX_SUSPENDED] = &usb_switch_en_susp_cfg,
        },
    },
};

static struct msm_gpiomux_config msm8x60_snd_configs[] __initdata = {
	{
		.gpio = 108,
		.settings = {
			[GPIOMUX_ACTIVE]    = &msm_snddev_active_config,
			[GPIOMUX_SUSPENDED] = &msm_snddev_suspend_config,
		},
	},
	{
		.gpio = 109,
		.settings = {
			[GPIOMUX_ACTIVE]    = &msm_snddev_active_config,
			[GPIOMUX_SUSPENDED] = &msm_snddev_suspend_config,
		},
	},
};

static struct msm_gpiomux_config msm8x60_mi2s_configs[] __initdata = {
	/* MI2S WS */
	{
		.gpio = 101,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mi2s_active_cfg,
			[GPIOMUX_SUSPENDED] = &mi2s_suspend_cfg,
		},
	},
	/* MI2S SCLK */
	{
		.gpio = 102,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mi2s_active_cfg,
			[GPIOMUX_SUSPENDED] = &mi2s_suspend_cfg,
		},
	},
	/* MI2S MCLK */
	{
		.gpio = 103,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mi2s_active_cfg,
			[GPIOMUX_SUSPENDED] = &mi2s_suspend_cfg,
		},
	},
	/* MI2S SD3 */
	{
		.gpio = 107,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mi2s_active_cfg,
			[GPIOMUX_SUSPENDED] = &mi2s_suspend_cfg,
		},
	},
};

static struct msm_gpiomux_config msm8x60_lcdc_configs[] __initdata = {
	/* lcdc_pclk */
	{
		.gpio = 0,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			/* [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg, */
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_hsync */
	{
		.gpio = 1,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_vsync */
	{
		.gpio = 2,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_den */
	{
		.gpio = 3,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
		},
	},
	/* lcdc_red7 */
	{
		.gpio = 4,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg_2m,
		},
	},
	/* lcdc_red6 */
	{
		.gpio = 5,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg_2m,
		},
	},
	/* lcdc_red5 */
	{
		.gpio = 6,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_red4 */
	{
		.gpio = 7,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_red3 */
	{
		.gpio = 8,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_red2 */
	{
		.gpio = 9,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_red1 */
	{
		.gpio = 10,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_red0 */
	{
		.gpio = 11,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_grn7 */
	{
		.gpio = 12,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg_2m,
		},
	},
	/* lcdc_grn6 */
	{
		.gpio = 13,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg_2m,
		},
	},
	/* lcdc_grn5 */
	{
		.gpio = 14,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_grn4 */
	{
		.gpio = 15,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_grn3 */
	{
		.gpio = 16,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_grn2 */
	{
		.gpio = 17,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_grn1 */
	{
		.gpio = 18,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_grn0 */
	{
		.gpio = 19,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_blu7 */
	{
		.gpio = 20,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg_2m,
		},
	},
	/* lcdc_blu6 */
	{
		.gpio = 21,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg_2m,
		},
	},
	/* lcdc_blu5 */
	{
		.gpio = 22,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_blu4 */
	{
		.gpio = 23,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_blu3 */
	{
		.gpio = 24,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_blu2 */
	{
		.gpio = 25,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_blu1 */
	{
		.gpio = 26,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
	/* lcdc_blu0 */
	{
		.gpio = 27,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
			[GPIOMUX_SUSPENDED] = &lcdc_active_cfg,
		},
	},
};

/* SAM: Added by me from HP Kernel 2.6.35 */
static struct msm_gpiomux_config msm8x60_lcdc_steady_configs[] = {
    /* lcdc_pclk */
    {
        .gpio = 0,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_hsync */
    {
        .gpio = 1,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_vsync */
    {
        .gpio = 2,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_den */
    {
        .gpio = 3,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red7 */
    {
        .gpio = 4,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red6 */
    {
        .gpio = 5,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red5 */
    {
        .gpio = 6,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red4 */
    {
        .gpio = 7,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red3 */
    {
        .gpio = 8,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red2 */
    {
        .gpio = 9,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red1 */
    {
        .gpio = 10,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_red0 */
    {
        .gpio = 11,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn7 */
    {
        .gpio = 12,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn6 */
    {
        .gpio = 13,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn5 */
    {
        .gpio = 14,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn4 */
    {
        .gpio = 15,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn3 */
    {
        .gpio = 16,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn2 */
    {
        .gpio = 17,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn1 */
    {
        .gpio = 18,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_grn0 */
    {
        .gpio = 19,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu7 */
    {
        .gpio = 20,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu6 */
    {
        .gpio = 21,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg_2m,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu5 */
    {
        .gpio = 22,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu4 */
    {
        .gpio = 23,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu3 */
    {
        .gpio = 24,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu2 */
    {
        .gpio = 25,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu1 */
    {
        .gpio = 26,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
    /* lcdc_blu0 */
    {
        .gpio = 27,
        .settings = {
            [GPIOMUX_ACTIVE]    = &lcdc_active_cfg,
            [GPIOMUX_SUSPENDED] = &lcdc_suspend_cfg,
        },
    },
};

static struct msm_gpiomux_config msm8x60_mdp_vsync_configs[] __initdata = {
	{
		.gpio = 28,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mdp_vsync_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdp_vsync_suspend_cfg,
		},
	},
};

static struct msm_gpiomux_config msm8x60_hdmi_configs[] __initdata = {
	{
		.gpio = 169,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 170,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 171,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 172,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_3_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
};

/* Because PMIC drivers do not use gpio-management routines and PMIC
 * gpios must never sleep, a "good enough" config is obtained by placing
 * the active config in the 'suspended' slot and leaving the active
 * config invalid: the suspended config will be installed at boot
 * and never replaced.
 */

static struct msm_gpiomux_config msm8x60_pmic_configs[] __initdata = {
	{
		.gpio = PMIC1_APC_USR_IRQ_N,
		.settings = {
			[GPIOMUX_ACTIVE] = &pmic_active_cfg,
			[GPIOMUX_SUSPENDED] = &pmic_suspended_cfg,
		},
	},
	{
		.gpio = PMIC2_APC_USR_IRQ_N,
		.settings = {
			[GPIOMUX_ACTIVE] = &pmic_active_cfg,
			[GPIOMUX_SUSPENDED] = &pmic_suspended_cfg,
		},
	},
};

static struct msm_gpiomux_config msm8x60_common_configs[] __initdata = {
	/* MDM2AP_STATUS */
	{
		.gpio = 77,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mdm2ap_status_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdm2ap_status_suspend_cfg,
		},
	},
#if 0
	/* PS_HOLD */
	{
		.gpio      = 92,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ps_hold,
		},
	},
#endif
};

#ifdef CONFIG_MT9M113
static struct msm_gpiomux_config msm8x60_cam_configs[] __initdata = {
#if 0
	{
		.gpio = 29,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_cfg,
		},
	},
	{
		.gpio = 30,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_cfg,
		},
	},
	{
		.gpio = 31,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_cfg,
		},
	},
	{
		.gpio = 32,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_5_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_cfg,
		},
	},
	{
		.gpio = 42,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_cfg,
		},
	},
#endif
	{
		.gpio = TENDERLOIN_CAM_I2C_DATA,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_3_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_3_cfg,
		},
	},
	{
		.gpio = TENDERLOIN_CAM_I2C_CLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_3_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_3_cfg,
		},
	},
	{
		.gpio = TENDERLOIN_CAMIF_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_4_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_cfg,
		},
	},
	{
		.gpio = TENDERLOIN_WEBCAM_RST,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_5_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_5_cfg,
		},
	},
	{
		.gpio = TENDERLOIN_WEBCAM_PWDN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_active_5_cfg,
			[GPIOMUX_SUSPENDED] = &cam_suspend_5_cfg,
		},
	},
};
#endif

static struct msm_gpiomux_config msm8x60_charm_configs[] __initdata = {
	/* AP2MDM_WAKEUP */
	{
		.gpio = 135,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* MDM2AP_VFR */
	{
		.gpio = 94,
		.settings = {
			[GPIOMUX_ACTIVE] = &mdm2ap_vfr_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdm2ap_vfr_suspend_cfg,
		}
	},
	/* AP2MDM_STATUS */
	{
		.gpio = 136,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* MDM2AP_STATUS */
	{
		.gpio = 134,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_status_cfg,
		}
	},
	/* MDM2AP_WAKEUP */
	{
		.gpio = 40,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* MDM2AP_ERRFATAL */
	{
		.gpio = 133,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_errfatal_cfg,
		}
	},
	/* AP2MDM_ERRFATAL */
	{
		.gpio = 93,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* AP2MDM_KPDPWR_N */
	{
		.gpio = 132,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_kpdpwr_n_cfg,
		}
	},
	/* AP2MDM_PMIC_RESET_N */
	{
		.gpio = 131,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_kpdpwr_n_cfg,
		}
	}
};

static struct gpiomux_setting bt_rst_n_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting bt_rst_n_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting bt_power_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting bt_power_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_KEEPER,
};

static struct gpiomux_setting bt_wake_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting bt_wake_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting bt_host_wake_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting bt_host_wake_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm8x60_bt_configs[] __initdata = {
    {
        .gpio = BT_RST_N,
        .settings = {
            [GPIOMUX_ACTIVE] = &bt_rst_n_active_cfg,
            [GPIOMUX_SUSPENDED] = &bt_rst_n_suspended_cfg,
        }
    },
    {
        .gpio = BT_POWER,
        .settings = {
            [GPIOMUX_ACTIVE] = &bt_power_active_cfg,
            [GPIOMUX_SUSPENDED] = &bt_power_suspended_cfg,
        }
    },
    {
        .gpio = BT_WAKE,
        .settings = {
            [GPIOMUX_ACTIVE] = &bt_wake_active_cfg,
            [GPIOMUX_SUSPENDED] = &bt_wake_suspended_cfg,
        }
    },
    {
        .gpio = BT_HOST_WAKE,
        .settings = {
            [GPIOMUX_ACTIVE] = &bt_host_wake_active_cfg,
            [GPIOMUX_SUSPENDED] = &bt_host_wake_suspended_cfg,
        }
    },
};
/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_bt_configs_3g[] __initdata = {

    GPIOMUX_ACT_SUSP_DEF(BT_RST_N_3G, \
            BT_RST_N_ACTIVE_CFG, BT_RST_N_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(BT_POWER_3G, \
            BT_POWER_ACTIVE_CFG, BT_POWER_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(BT_WAKE_3G, \
            BT_WAKE_ACTIVE_CFG, BT_WAKE_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(BT_HOST_WAKE_3G, \
            BT_HOST_WAKE_ACTIVE_CFG, BT_HOST_WAKE_SUSPENDED_CFG),

};
#endif

/* SAM: Add support for Keyboard GPIO PE */
#ifdef CONFIG_KEYBOARD_GPIO
static struct gpiomux_setting kbdgpio_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting kbdgpio_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm8x60_kbdgpio_cfgs[] __initdata = {
    {
        .gpio = VOL_DN_GPIO,
        .settings = {
            [GPIOMUX_ACTIVE] = &kbdgpio_active_cfg,
            [GPIOMUX_SUSPENDED] = &kbdgpio_suspended_cfg,
        }
    },
    {
        .gpio = VOL_UP_GPIO,
        .settings = {
            [GPIOMUX_ACTIVE] = &kbdgpio_active_cfg,
            [GPIOMUX_SUSPENDED] = &kbdgpio_suspended_cfg,
        }
    },
    {
        .gpio = CORE_NAVI_GPIO,
        .settings = {
            [GPIOMUX_ACTIVE] = &kbdgpio_active_cfg,
            [GPIOMUX_SUSPENDED] = &kbdgpio_suspended_cfg,
        }
    },
    {
        .gpio = JACK_DET_GPIO,
        .settings = {
            [GPIOMUX_ACTIVE] = &kbdgpio_active_cfg,
            [GPIOMUX_SUSPENDED] = &kbdgpio_suspended_cfg,
        }
    },
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_kbdgpio_cfgs_3g[] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(CORE_NAVI_GPIO, GPIO_IN_8M_PU, GPIO_IN_2M_PU),
};
#endif

#endif

#ifdef CONFIG_CHARGER_MAX8903
static struct gpiomux_setting max8903b_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting max8903b_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm8x60_charger_cfgs[] __initdata = {
    {
        .gpio = MAX8903B_GPIO_DC_OK,
        .settings = {
            [GPIOMUX_ACTIVE] = &max8903b_active_cfg,
            [GPIOMUX_SUSPENDED] = &max8903b_suspended_cfg,
        }
    },
    {
        .gpio = MAX8903B_GPIO_STATUS_N,
        .settings = {
            [GPIOMUX_ACTIVE] = &max8903b_active_cfg,
            [GPIOMUX_SUSPENDED] = &max8903b_suspended_cfg,
        }
    },
    {
        .gpio = MAX8903B_GPIO_FAULT_N,
        .settings = {
            [GPIOMUX_ACTIVE] = &max8903b_active_cfg,
            [GPIOMUX_SUSPENDED] = &max8903b_suspended_cfg,
        }
    },
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_charger_cfgs_3g[] __initdata = {
    //GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_DC_CHG_MODE,   GPIO_OUTH_8M_PN, GPIO_OUTH_8M_PN),
    //GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_USB_CHG_MODE_3G,   GPIO_OUTH_8M_PN, GPIO_OUTH_8M_PN),
    //GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_USB_CHG_SUS,   GPIO_OUTL_8M_PN, GPIO_OUTL_8M_PN),
    //GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_CHG_D_ISET_1,  GPIO_OUTL_8M_PN, GPIO_OUTL_8M_PN),
    //GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_CHG_D_ISET_2,  GPIO_OUTL_8M_PN, GPIO_OUTL_8M_PN),
    //GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_CHG_EN,        GPIO_OUTL_8M_PN, GPIO_OUTL_8M_PN),
    GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_DC_OK_3G,        GPIO_IN_8M_PU,   GPIO_IN_8M_PU),
    GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_STATUS_N,        GPIO_IN_8M_PU,   GPIO_IN_8M_PU),
    GPIOMUX_ACT_SUSP_DEF(MAX8903B_GPIO_FAULT_N,         GPIO_IN_8M_PU,   GPIO_IN_8M_PU),
};
#endif

#endif

/* SAM: Add or find support for bewlo touchscreen */
#ifdef CONFIG_TOUCHSCREEN_MXT1386_I2C
static struct gpiomux_setting touchscreen_active_cfg = {
    .func = GPIOMUX_FUNC_1,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting touchscreen_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm8x60_touchscreen_cfgs[] __initdata = {
    {
        .gpio = 72,
        .settings = {
            [GPIOMUX_ACTIVE] = &touchscreen_active_cfg,
            [GPIOMUX_SUSPENDED] = &touchscreen_suspended_cfg,
        }
    },
    {
        .gpio = 73,
        .settings = {
            [GPIOMUX_ACTIVE] = &touchscreen_active_cfg,
            [GPIOMUX_SUSPENDED] = &touchscreen_suspended_cfg,
        }
    },
};
#endif

static struct gpiomux_setting lm8502_lighting_int_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting lm8502_lighting_int_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting lm8502_lighting_en_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting lm8502_lighting_en_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config msm8x60_lighting_cfgs[] __initdata = {
    {
        .gpio = LM8502_LIGHTING_INT_IRQ_GPIO,
        .settings = {
            [GPIOMUX_ACTIVE] = &lm8502_lighting_int_active_cfg,
            [GPIOMUX_SUSPENDED] = &lm8502_lighting_int_suspended_cfg,
        }
    },
    {
        .gpio = LM8502_LIGHTING_EN_GPIO,
        .settings = {
            [GPIOMUX_ACTIVE] = &lm8502_lighting_en_active_cfg,
            [GPIOMUX_SUSPENDED] = &lm8502_lighting_en_suspended_cfg,
        }
    },
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_lighting_cfgs_3g[] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(LM8502_LIGHTING_INT_IRQ_GPIO_3G, GPIO_IN_8M_PU, GPIO_IN_2M_PU),
    GPIOMUX_ACT_SUSP_DEF(LM8502_LIGHTING_EN_GPIO, GPIO_OUTL_8M_PN, GPIO_OUTL_2M_PN)
};
#endif

static struct gpiomux_setting tenderloin_gyro_int_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting tenderloin_gyro_int_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting tenderloin_gsens_int_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting tenderloin_gsens_int_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm8x60_sensor_cfgs[] __initdata = {
    {
        .gpio = TENDERLOIN_GYRO_INT,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_gyro_int_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_gyro_int_suspended_cfg,
        }
    },
    {
        .gpio = TENDERLOIN_GSENS_INT,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_gsens_int_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_gsens_int_suspended_cfg,
        }
    },
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_sensor_cfgs_3g[] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_GYRO_INT_3G, GPIO_IN_8M_PU, GPIO_IN_2M_PU),
};
#endif

static struct gpiomux_setting tenderloin_wl_host_wake_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting tenderloin_wl_host_wake_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting tenderloin_host_wake_wl_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};
static struct gpiomux_setting tenderloin_host_wake_wl_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting tenderloin_wlan_rst_n_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};
static struct gpiomux_setting tenderloin_wlan_rst_n_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_KEEPER,
};

/* wifi */
static struct msm_gpiomux_config msm8x60_wlan_configs[] __initdata = {
    {
        .gpio = TENDERLOIN_GPIO_WL_HOST_WAKE,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_wl_host_wake_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_wl_host_wake_suspended_cfg,
        }
    },
    {
        .gpio = TENDERLOIN_GPIO_HOST_WAKE_WL,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_host_wake_wl_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_host_wake_wl_suspended_cfg,
        }
    },
    {
        .gpio = TENDERLOIN_GPIO_WLAN_RST_N,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_wlan_rst_n_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_wlan_rst_n_suspended_cfg,
        }
    },
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_wlan_configs_3g[] __initdata = {

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_GPIO_WL_HOST_WAKE, \
            GPIO_IN_8M_PD, GPIO_IN_8M_PD),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_GPIO_HOST_WAKE_WL_3G, \
            GPIO_OUTH_8M_PN, GPIO_OUTH_2M_PN),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_GPIO_HOST_WAKE_WL_3G, \
            GPIO_OUTL_8M_PN, GPIO_IN_2M_PK),

};
#endif

static struct gpiomux_setting force_boot_dis_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config msm8x60_system_gpio_cfgs[] __initdata = {
    {
        .gpio = GPIO_FORCE_BOOT_DIS,
        .settings = {
            [GPIOMUX_ACTIVE] = &force_boot_dis_cfg,
            [GPIOMUX_SUSPENDED] = &force_boot_dis_cfg,
        }
    }
};

#ifdef CONFIG_A6

// Currently the suspend configuration for A6 is not used
#define TENDERLOIN_A6_TCK_ACTIVE_CFG GPIO_OUTH_8M_PN
#define TENDERLOIN_A6_TCK_SUSPENDED_CFG GPIO_IN_2M_PU

#define TENDERLOIN_A6_WAKEUP_ACTIVE GPIO_OUTL_8M_PN
#define TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG GPIO_OUTL_2M_PN

#define TENDERLOIN_A6_TDIO_ACTIVE_CFG GPIO_OUTH_8M_PU
#define TENDERLOIN_A6_TDIO_SUSPENDED_CFG GPIO_IN_2M_PU

#define TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG GPIO_IN_8M_PU
#define TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG GPIO_IN_2M_PU

/* a6 */
static struct msm_gpiomux_config msm8x60_a6_configs[] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TCK, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_WAKEUP, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TDIO, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_MSM_IRQ, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TCK, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_WAKEUP, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TDIO, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_MSM_IRQ, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_a6_configs_3g[] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TCK_3G, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_WAKEUP_3G, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TDIO_3G, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_MSM_IRQ_3G, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TCK_3G, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_WAKEUP_3G, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TDIO_3G, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_MSM_IRQ_3G, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),

};
#endif

static struct msm_gpiomux_config msm8x60_a6_configs_dvt [] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TCK, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_WAKEUP, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TDIO, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_MSM_IRQ_DVT, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TCK, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_WAKEUP, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TDIO, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_MSM_IRQ_DVT, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config msm8x60_a6_configs_3g_dvt [] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TCK_3G_DVT, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_WAKEUP_3G, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_TDIO_3G, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_0_MSM_IRQ_3G_DVT, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TCK_3G, \
            TENDERLOIN_A6_TCK_ACTIVE_CFG, TENDERLOIN_A6_TCK_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_WAKEUP_3G, \
            TENDERLOIN_A6_WAKEUP_ACTIVE, TENDERLOIN_A6_WAKEUP_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_TDIO_3G, \
            TENDERLOIN_A6_TDIO_ACTIVE_CFG, TENDERLOIN_A6_TDIO_SUSPENDED_CFG),

    GPIOMUX_ACT_SUSP_DEF(TENDERLOIN_A6_1_MSM_IRQ_3G_DVT, \
            TENDERLOIN_A6_MSM_IRQ_ACTIVE_CFG, TENDERLOIN_A6_MSM_IRQ_SUSPENDED_CFG),

};
#endif

#endif

#ifdef CONFIG_MSM8X60_AUDIO
static struct gpiomux_setting tenderloin_aud_en_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting tenderloin_aud_en_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct msm_gpiomux_config msm8x60_aud_configs[] __initdata = {
        {
        .gpio = TENDERLOIN_AUD_LDO1_EN,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_aud_en_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_aud_en_suspended_cfg,
        }
    },
    {
        .gpio = TENDERLOIN_AUD_LDO2_EN,
        .settings = {
            [GPIOMUX_ACTIVE] = &tenderloin_aud_en_active_cfg,
            [GPIOMUX_SUSPENDED] = &tenderloin_aud_en_suspended_cfg,
        }
    },
};
#endif

#if defined (CONFIG_TOUCHSCREEN_CY8CTMA395) \
    || defined (CONFIG_TOUCHSCREEN_CY8CTMA395_MODULE)
static struct gpiomux_setting gpio_ctp_rx_active_cfg = {
    .func = GPIOMUX_FUNC_1,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_ctp_rx_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gpio_cy8ctma395_xres_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting gpio_cy8ctma395_xres_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting gpio_ctp_wake_active_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
    .dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting gpio_ctp_wake_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gpio_ctp_scl_active_cfg = {
    .func = GPIOMUX_FUNC_1,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_ctp_scl_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_ctp_sda_active_cfg = {
    .func = GPIOMUX_FUNC_1,
    .drv = GPIOMUX_DRV_8MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_ctp_sda_suspended_cfg = {
    .func = GPIOMUX_FUNC_GPIO,
    .drv = GPIOMUX_DRV_2MA,
    .pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config ctp_configs[] __initdata = {
    {
        .gpio = GPIO_CTP_RX,
        .settings = {
            [GPIOMUX_ACTIVE] = &gpio_ctp_rx_active_cfg,
            [GPIOMUX_SUSPENDED] = &gpio_ctp_rx_suspended_cfg,
        }
    },
    {
        .gpio = GPIO_CY8CTMA395_XRES,
        .settings = {
            [GPIOMUX_ACTIVE] = &gpio_cy8ctma395_xres_active_cfg,
            [GPIOMUX_SUSPENDED] = &gpio_cy8ctma395_xres_suspended_cfg,
        }
    },
    {
        .gpio = GPIO_CTP_WAKE,
        .settings = {
            [GPIOMUX_ACTIVE] = &gpio_ctp_wake_active_cfg,
            [GPIOMUX_SUSPENDED] = &gpio_ctp_wake_suspended_cfg,
        }
    },
    {
        .gpio = GPIO_CTP_SCL,
        .settings = {
            [GPIOMUX_ACTIVE] = &gpio_ctp_scl_active_cfg,
            [GPIOMUX_SUSPENDED] = &gpio_ctp_scl_suspended_cfg,
        }
    },
    {
        .gpio = GPIO_CTP_SDA,
        .settings = {
            [GPIOMUX_ACTIVE] = &gpio_ctp_sda_active_cfg,
            [GPIOMUX_SUSPENDED] = &gpio_ctp_sda_suspended_cfg,
        }
    },
};

/* SAM: Temporary disable 3G support */
#if 0
static struct msm_gpiomux_config ctp_configs_3g[] __initdata = {
    GPIOMUX_ACT_SUSP_DEF(GPIO_CTP_RX,
        GPIOMUX_CFG(GPIOMUX_FUNC_1, GPIOMUX_DRV_2MA, GPIOMUX_PULL_NONE),
        GPIO_IN_2M_PD),
    GPIOMUX_ACT_SUSP_DEF(GPIO_CY8CTMA395_XRES,
            GPIO_OUTH_8M_PN, GPIO_OUTH_2M_PN),
    GPIOMUX_ACT_SUSP_DEF(GPIO_CTP_WAKE_3G,
            GPIO_OUTH_8M_PN, GPIO_IN_2M_PU),
    GPIOMUX_ACT_SUSP_DEF(GPIO_CTP_SCL,
            GPIOMUX_CFG(GPIOMUX_FUNC_1, GPIOMUX_DRV_8MA, GPIOMUX_PULL_NONE),
            GPIO_IN_2M_PN),
    GPIOMUX_ACT_SUSP_DEF(GPIO_CTP_SDA,
            GPIOMUX_CFG(GPIOMUX_FUNC_1, GPIOMUX_DRV_8MA, GPIOMUX_PULL_NONE),
            GPIO_IN_2M_PN),
};
#endif

#endif /* CONFIG_TOUCHSCREEN_CY8CTMA395[_MODULE] */

struct msm_gpiomux_configs
tenderloin_gpiomux_cfgs[] __initdata = {
    {msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
    {msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
    {msm8x60_bt_configs, ARRAY_SIZE(msm8x60_bt_configs)},
    {msm8x60_wlan_configs, ARRAY_SIZE(msm8x60_wlan_configs)},
    {msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
    {msm8x60_sensor_cfgs, ARRAY_SIZE(msm8x60_sensor_cfgs)},
    {msm8x60_lighting_cfgs, ARRAY_SIZE(msm8x60_lighting_cfgs)},
#ifdef CONFIG_KEYBOARD_GPIO
    {msm8x60_kbdgpio_cfgs, ARRAY_SIZE(msm8x60_kbdgpio_cfgs)},
#endif
#ifdef CONFIG_CHARGER_MAX8903
    {msm8x60_charger_cfgs, ARRAY_SIZE(msm8x60_charger_cfgs)},
#endif
#ifdef CONFIG_TOUCHSCREEN_MXT1386_I2C
    {msm8x60_touchscreen_cfgs, ARRAY_SIZE(msm8x60_touchscreen_cfgs)},
#endif
#ifdef CONFIG_A6
    {msm8x60_a6_configs, ARRAY_SIZE(msm8x60_a6_configs)},
#endif
#ifdef CONFIG_WEBCAM_MT9M113
    {msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
#ifdef CONFIG_MSM8X60_AUDIO
    {msm8x60_aud_configs, ARRAY_SIZE(msm8x60_aud_configs)},
    {msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
#endif

    {msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},
    {msm8x60_system_gpio_cfgs, ARRAY_SIZE(msm8x60_system_gpio_cfgs)},


#if 0
    {msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
    {msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
    {msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
    {msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},

    {msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
    {msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
    {msm8x60_charm_sdc_configs, ARRAY_SIZE(msm8x60_charm_sdc_configs)},
#endif
#if defined (CONFIG_TOUCHSCREEN_CY8CTMA395) \
    || defined (CONFIG_TOUCHSCREEN_CY8CTMA395_MODULE)
    {ctp_configs, ARRAY_SIZE(ctp_configs)},
#endif /* CONFIG_TOUCHSCREEN_CY8CTMA395[_MODULE] */
    {NULL, 0},
};

/* SAM: Temporary disable 3G support */
#if 0
struct msm_gpiomux_configs
tenderloin_3g_gpiomux_cfgs[] __initdata = {
    {msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
    {msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
    {msm8x60_bt_configs_3g, ARRAY_SIZE(msm8x60_bt_configs_3g)},
    {msm8x60_wlan_configs_3g, ARRAY_SIZE(msm8x60_wlan_configs_3g)},
    {msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
    {msm8x60_sensor_cfgs_3g, ARRAY_SIZE(msm8x60_sensor_cfgs_3g)},
    {msm8x60_lighting_cfgs_3g, ARRAY_SIZE(msm8x60_lighting_cfgs_3g)},

#ifdef CONFIG_KEYBOARD_GPIO
    {msm8x60_kbdgpio_cfgs_3g, ARRAY_SIZE(msm8x60_kbdgpio_cfgs)},
#endif
#ifdef CONFIG_CHARGER_MAX8903
    {msm8x60_charger_cfgs_3g, ARRAY_SIZE(msm8x60_charger_cfgs_3g)},
#endif
#ifdef CONFIG_TOUCHSCREEN_MXT1386_I2C
    {msm8x60_touchscreen_cfgs, ARRAY_SIZE(msm8x60_touchscreen_cfgs)},
#endif
#ifdef CONFIG_A6
    {msm8x60_a6_configs_3g, ARRAY_SIZE(msm8x60_a6_configs)},
#endif
#ifdef CONFIG_WEBCAM_MT9M113
    {msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
#ifdef CONFIG_MSM8X60_AUDIO
    {msm8x60_aud_configs, ARRAY_SIZE(msm8x60_aud_configs)},
    {msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
#endif
#ifdef CONFIG_USB_PEHCI_HCD
    {msm8x60_isp_usb_configs, ARRAY_SIZE(msm8x60_isp_usb_configs)},
    {msm8x60_ebi2_configs, ARRAY_SIZE(msm8x60_ebi2_configs)},
#endif
    {msm8x60_system_gpio_cfgs, ARRAY_SIZE(msm8x60_system_gpio_cfgs)},

#if 0
    {msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},

    {msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
    {msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
    {msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
    {msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
    {msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},

    {msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
    {msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
    {msm8x60_charm_sdc_configs, ARRAY_SIZE(msm8x60_charm_sdc_configs)},
#endif
#if defined (CONFIG_TOUCHSCREEN_CY8CTMA395) \
    || defined (CONFIG_TOUCHSCREEN_CY8CTMA395_MODULE)
    {ctp_configs_3g, ARRAY_SIZE(ctp_configs_3g)},
#endif /* CONFIG_TOUCHSCREEN_CY8CTMA395[_MODULE] */

    {NULL, 0},
};
#endif

struct msm_gpiomux_configs
tenderloin_dvt_gpiomux_cfgs[] __initdata = {
    {msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
    {msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
    {msm8x60_bt_configs, ARRAY_SIZE(msm8x60_bt_configs)},
    {msm8x60_wlan_configs, ARRAY_SIZE(msm8x60_wlan_configs)},
    {msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
    {msm8x60_sensor_cfgs, ARRAY_SIZE(msm8x60_sensor_cfgs)},
    {msm8x60_lighting_cfgs, ARRAY_SIZE(msm8x60_lighting_cfgs)},
#ifdef CONFIG_KEYBOARD_GPIO
    {msm8x60_kbdgpio_cfgs, ARRAY_SIZE(msm8x60_kbdgpio_cfgs)},
#endif
#ifdef CONFIG_CHARGER_MAX8903
    {msm8x60_charger_cfgs, ARRAY_SIZE(msm8x60_charger_cfgs)},
#endif
#ifdef CONFIG_TOUCHSCREEN_MXT1386_I2C
    {msm8x60_touchscreen_cfgs, ARRAY_SIZE(msm8x60_touchscreen_cfgs)},
#endif
#ifdef CONFIG_A6
    {msm8x60_a6_configs_dvt, ARRAY_SIZE(msm8x60_a6_configs_dvt)},
#endif
#ifdef CONFIG_WEBCAM_MT9M113
    {msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
#ifdef CONFIG_MSM8X60_AUDIO
    {msm8x60_aud_configs, ARRAY_SIZE(msm8x60_aud_configs)},
    {msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
#endif
    {msm8x60_system_gpio_cfgs, ARRAY_SIZE(msm8x60_system_gpio_cfgs)},

#if 0
    {msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},

    {msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
    {msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
    {msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
    {msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},

    {msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
    {msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
    {msm8x60_charm_sdc_configs, ARRAY_SIZE(msm8x60_charm_sdc_configs)},
#endif
#if defined (CONFIG_TOUCHSCREEN_CY8CTMA395) \
    || defined (CONFIG_TOUCHSCREEN_CY8CTMA395_MODULE)
    {ctp_configs, ARRAY_SIZE(ctp_configs)},
#endif /* CONFIG_TOUCHSCREEN_CY8CTMA395[_MODULE] */
    {NULL, 0},
};

/* SAM: Temporary disable 3G support */
#if 0
struct msm_gpiomux_configs
tenderloin_3g_dvt_gpiomux_cfgs[] __initdata = {
    {msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
    {msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
    {msm8x60_bt_configs_3g, ARRAY_SIZE(msm8x60_bt_configs_3g)},
    {msm8x60_wlan_configs_3g, ARRAY_SIZE(msm8x60_wlan_configs_3g)},
    {msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
    {msm8x60_sensor_cfgs_3g, ARRAY_SIZE(msm8x60_sensor_cfgs_3g)},
    {msm8x60_lighting_cfgs_3g, ARRAY_SIZE(msm8x60_lighting_cfgs_3g)},

#ifdef CONFIG_KEYBOARD_GPIO
    {msm8x60_kbdgpio_cfgs_3g, ARRAY_SIZE(msm8x60_kbdgpio_cfgs)},
#endif
#ifdef CONFIG_CHARGER_MAX8903
    {msm8x60_charger_cfgs_3g, ARRAY_SIZE(msm8x60_charger_cfgs_3g)},
#endif
#ifdef CONFIG_TOUCHSCREEN_MXT1386_I2C
    {msm8x60_touchscreen_cfgs, ARRAY_SIZE(msm8x60_touchscreen_cfgs)},
#endif
#ifdef CONFIG_A6
    {msm8x60_a6_configs_3g_dvt, ARRAY_SIZE(msm8x60_a6_configs_3g_dvt)},
#endif
#ifdef CONFIG_WEBCAM_MT9M113
    {msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
#ifdef CONFIG_MSM8X60_AUDIO
    {msm8x60_aud_configs, ARRAY_SIZE(msm8x60_aud_configs)},
    {msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
#endif
#ifdef CONFIG_USB_PEHCI_HCD
    {msm8x60_isp_usb_configs, ARRAY_SIZE(msm8x60_isp_usb_configs)},
    {msm8x60_ebi2_configs, ARRAY_SIZE(msm8x60_ebi2_configs)},
#endif

    {msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},
    {msm8x60_system_gpio_cfgs, ARRAY_SIZE(msm8x60_system_gpio_cfgs)},

#if 0
    {msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
    {msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
    {msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
    {msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
    {msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},

    {msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
    {msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
    {msm8x60_charm_sdc_configs, ARRAY_SIZE(msm8x60_charm_sdc_configs)},
#endif
#if defined (CONFIG_TOUCHSCREEN_CY8CTMA395) \
    || defined (CONFIG_TOUCHSCREEN_CY8CTMA395_MODULE)
    {ctp_configs_3g, ARRAY_SIZE(ctp_configs_3g)},
#endif /* CONFIG_TOUCHSCREEN_CY8CTMA395[_MODULE] */

    {NULL, 0},
};
#endif

/* SAM: DELME: Delete below when board file is added */
struct msm_gpiomux_configs
msm8x60_surf_ffa_gpiomux_cfgs[] __initdata = {
	{msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},
	{msm8x60_ebi2_configs, ARRAY_SIZE(msm8x60_ebi2_configs)},
	{msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
	{msm8x60_isp_usb_configs, ARRAY_SIZE(msm8x60_isp_usb_configs)},
#endif
	{msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
	{msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
	{msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
	{msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
	{msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},
	{msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
	{msm8x60_mdp_vsync_configs, ARRAY_SIZE(msm8x60_mdp_vsync_configs)},
	{msm8x60_hdmi_configs, ARRAY_SIZE(msm8x60_hdmi_configs)},
	{msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
	{msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
#ifdef CONFIG_MT9M113
	{msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
	{msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
	{NULL, 0},
};

struct msm_gpiomux_configs
msm8x60_fluid_gpiomux_cfgs[] __initdata = {
	{msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},
	{msm8x60_fluid_gsbi_configs, ARRAY_SIZE(msm8x60_fluid_gsbi_configs)},
	{msm8x60_ebi2_configs, ARRAY_SIZE(msm8x60_ebi2_configs)},
	{msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
	{msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
	{msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
	{msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
	{msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
	{msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},
	{msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
	{msm8x60_mdp_vsync_configs, ARRAY_SIZE(msm8x60_mdp_vsync_configs)},
	{msm8x60_hdmi_configs, ARRAY_SIZE(msm8x60_hdmi_configs)},
	{msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
	{msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
#ifdef CONFIG_MT9M113
	{msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
	{msm8x60_tma300_configs, ARRAY_SIZE(msm8x60_tma300_configs)},
	{NULL, 0},
};

struct msm_gpiomux_configs
msm8x60_charm_gpiomux_cfgs[] __initdata = {
	{msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},
	{msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
#ifdef CONFIG_MSM_GSBI9_UART
	{msm8x60_charm_uart_configs, ARRAY_SIZE(msm8x60_charm_uart_configs)},
#endif
	{msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
	{msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
	{msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
	{msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
	{msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},
	{msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
	{msm8x60_mdp_vsync_configs, ARRAY_SIZE(msm8x60_mdp_vsync_configs)},
	{msm8x60_hdmi_configs, ARRAY_SIZE(msm8x60_hdmi_configs)},
	{msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
	{msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
#ifdef CONFIG_MT9M113
	{msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
	{msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
	{msm8x60_charm_sdc_configs, ARRAY_SIZE(msm8x60_charm_sdc_configs)},
	{msm8x60_charm_configs, ARRAY_SIZE(msm8x60_charm_configs)},
	{NULL, 0},
};

struct msm_gpiomux_configs
msm8x60_dragon_gpiomux_cfgs[] __initdata = {
	{msm8x60_gsbi_configs, ARRAY_SIZE(msm8x60_gsbi_configs)},
	{msm8x60_ebi2_configs, ARRAY_SIZE(msm8x60_ebi2_configs)},
	{msm8x60_uart_configs, ARRAY_SIZE(msm8x60_uart_configs)},
#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
	{msm8x60_isp_usb_configs, ARRAY_SIZE(msm8x60_isp_usb_configs)},
#endif
	{msm8x60_ts_configs, ARRAY_SIZE(msm8x60_ts_configs)},
	{msm8x60_aux_pcm_configs, ARRAY_SIZE(msm8x60_aux_pcm_configs)},
	{msm8x60_sdc_configs, ARRAY_SIZE(msm8x60_sdc_configs)},
	{msm8x60_snd_configs, ARRAY_SIZE(msm8x60_snd_configs)},
	{msm8x60_mi2s_configs, ARRAY_SIZE(msm8x60_mi2s_configs)},
	{msm8x60_lcdc_configs, ARRAY_SIZE(msm8x60_lcdc_configs)},
	{msm8x60_mdp_vsync_configs, ARRAY_SIZE(msm8x60_mdp_vsync_configs)},
	{msm8x60_hdmi_configs, ARRAY_SIZE(msm8x60_hdmi_configs)},
	{msm8x60_pmic_configs, ARRAY_SIZE(msm8x60_pmic_configs)},
	{msm8x60_common_configs, ARRAY_SIZE(msm8x60_common_configs)},
#ifdef CONFIG_MT9M113
	{msm8x60_cam_configs, ARRAY_SIZE(msm8x60_cam_configs)},
#endif
	{msm8x60_tmg200_configs, ARRAY_SIZE(msm8x60_tmg200_configs)},
	{NULL, 0},
};

void __init msm8x60_init_gpiomux(struct msm_gpiomux_configs *cfgs)
{
	int rc;

	rc = msm_gpiomux_init(NR_GPIO_IRQS);
	if (rc) {
		pr_err("%s failure: %d\n", __func__, rc);
		return;
	}

	while (cfgs->cfg) {
		msm_gpiomux_install(cfgs->cfg, cfgs->ncfg);
		++cfgs;
	}
}

void msm8x60_gpiomux_lcdc_steadycfg(void)
{
    msm_gpiomux_install(msm8x60_lcdc_steady_configs,
            ARRAY_SIZE(msm8x60_lcdc_steady_configs));
}
