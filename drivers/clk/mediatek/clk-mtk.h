/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2018 MediaTek Inc.
 * Author: Ryder Lee <ryder.lee@mediatek.com>
 */

#ifndef __DRV_CLK_MTK_H
#define __DRV_CLK_MTK_H

#define CLK_XTAL			0
#define MHZ				(1000 * 1000)

#define HAVE_RST_BAR			BIT(0)
#define CLK_DOMAIN_SCPSYS		BIT(0)

#define CLK_GATE_SETCLR			BIT(0)
#define CLK_GATE_SETCLR_INV		BIT(1)
#define CLK_GATE_NO_SETCLR		BIT(2)
#define CLK_GATE_NO_SETCLR_INV		BIT(3)
#define CLK_GATE_MASK			GENMASK(3, 0)

#define CLK_PARENT_APMIXED		BIT(4)
#define CLK_PARENT_TOPCKGEN		BIT(5)
#define CLK_PARENT_MASK			GENMASK(5, 4)

struct mtk_pll_data {
	const int id;
	u32 reg;
	u32 pwr_reg;
	u32 en_mask;
	u32 pd_reg;
	int pd_shift;
	u32 flags;
	u32 rst_bar_mask;
	u64 fmax;
	int pcwbits;
	u32 pcw_reg;
	int pcw_shift;
};

struct mtk_fixed_clk {
	const int id;
	const int parent;
	unsigned long rate;
};

#define FIXED_CLK(_id, _parent, _rate) {		\
		.id = _id,				\
		.parent = _parent,			\
		.rate = _rate,				\
	}

struct mtk_fixed_factor {
	const int id;
	const int parent;
	u32 mult;
	u32 div;
	u32 flags;
};

#define FACTOR(_id, _parent, _mult, _div, _flags) {	\
		.id = _id,				\
		.parent = _parent,			\
		.mult = _mult,				\
		.div = _div,				\
		.flags = _flags,			\
	}

struct mtk_composite {
	const int id;
	const int *parent;
	u32 mux_reg;
	u32 gate_reg;
	u32 mux_mask;
	signed char mux_shift;
	signed char gate_shift;
	signed char num_parents;
	u16 flags;
};

#define MUX_GATE_FLAGS(_id, _parents, _reg, _shift, _width, _gate,	\
		       _flags) {					\
		.id = _id,						\
		.mux_reg = _reg,					\
		.mux_shift = _shift,					\
		.mux_mask = BIT(_width) - 1,				\
		.gate_reg = _reg,					\
		.gate_shift = _gate,					\
		.parent = _parents,					\
		.num_parents = ARRAY_SIZE(_parents),			\
		.flags = _flags,					\
	}

#define MUX_GATE(_id, _parents, _reg, _shift, _width, _gate)		\
	MUX_GATE_FLAGS(_id, _parents, _reg, _shift, _width, _gate, 0)

#define MUX(_id, _parents, _reg, _shift, _width) {			\
		.id = _id,						\
		.mux_reg = _reg,					\
		.mux_shift = _shift,					\
		.mux_mask = BIT(_width) - 1,				\
		.gate_shift = -1,					\
		.parent = _parents,					\
		.num_parents = ARRAY_SIZE(_parents),			\
		.flags = 0,						\
	}

struct mtk_gate_regs {
	u32 sta_ofs;
	u32 clr_ofs;
	u32 set_ofs;
};

struct mtk_gate {
	const int id;
	const int parent;
	const struct mtk_gate_regs *regs;
	int shift;
	u32 flags;
};

struct mtk_clk_tree {
	unsigned long xtal_rate;
	unsigned long xtal2_rate;
	const struct mtk_pll_data *plls;
	const struct mtk_fixed_clk *fclks;
	const struct mtk_fixed_factor *fdivs;
	const struct mtk_composite *muxes;
};

struct mtk_clk_priv {
	void __iomem *base;
	const struct mtk_clk_tree *tree;
};

struct mtk_cg_priv {
	void __iomem *base;
	const struct mtk_clk_tree *tree;
	const struct mtk_gate *gates;
};

extern const struct clk_ops mtk_clk_apmixedsys_ops;
extern const struct clk_ops mtk_clk_topckgen_ops;
extern const struct clk_ops mtk_clk_gate_ops;

int mtk_clk_init(struct udevice *dev, const struct mtk_clk_tree *tree);
int mtk_clk_gate_init(struct udevice *dev,
		      const struct mtk_clk_tree *tree,
		      const struct mtk_gate *gates);

#endif /* __DRV_CLK_MTK_H */