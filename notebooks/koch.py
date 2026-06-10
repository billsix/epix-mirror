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
import epix
from epix import P

koch_seed = [6, 4, 0, 1, -1, 0]

# %%
with epix.figure(P(0, 0), P(4, 1), "6 x 1.5in") as fig:
    epix.degrees()

    for i in range(1, 5):
        # draw von Koch curve at "depth" i
        tmp = epix.screen(P(-1, -1), P(1, 1))
        epix.activate(tmp)

        if i == 1:
            epix.bold(epix.Red())
            epix.fractal(epix.cis(150), epix.cis(30), i, koch_seed)
            epix.plain(epix.Black())
        else:
            epix.fractal(epix.cis(150), epix.cis(30), i, koch_seed)

        epix.fractal(epix.cis(30), epix.cis(-90), i, koch_seed)
        epix.fractal(epix.cis(-90), epix.cis(150), i, koch_seed)

        epix.inset(tmp, P(i - 1, 0), P(i, 1))
        epix.deactivate(tmp)
fig
