# ---
# jupyter:
#   jupytext:
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#   kernelspec:
#     display_name: epix
#     language: python
#     name: epix
# ---

# %% [markdown]
# # koch — ported from `samples/koch.xp`
# The von Koch snowflake at recursion depths 1–4, each drawn from three
# `fractal` edges (seeded by `koch_seed`) and laid out left-to-right in its own
# `screen` panel.

# %%
from __future__ import annotations

import epix
from epix import Point

koch_seed = [6, 4, 0, 1, -1, 0]

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=4, y=1), size="6 x 1.5in"
) as fig:
    epix.degrees()

    for i in range(1, 5):
        # draw von Koch curve at "depth" i
        panel: epix.Screen = epix.Screen(
            lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
        )
        with epix.activated(panel):
            if i == 1:
                epix.bold(epix.red())
                epix.fractal(
                    p=epix.cis(150), q=epix.cis(30), depth=i, pre_seed=koch_seed
                )
                epix.plain(epix.black())
            else:
                epix.fractal(
                    p=epix.cis(150), q=epix.cis(30), depth=i, pre_seed=koch_seed
                )

            epix.fractal(p=epix.cis(30), q=epix.cis(-90), depth=i, pre_seed=koch_seed)
            epix.fractal(p=epix.cis(-90), q=epix.cis(150), depth=i, pre_seed=koch_seed)

            epix.inset(
                child=panel, lower_left=Point(x=i - 1, y=0), upper_right=Point(x=i, y=1)
            )
fig
