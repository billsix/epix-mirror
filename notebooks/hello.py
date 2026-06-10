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
# # ePiX in a notebook
#
# Phase 1 demo: render existing ePiX `.xp` figures inline. (Phase 2 will let you
# *build* the figure in Python; the same inline display is used either way.)

# %%
import epix

# %% [markdown]
# The classic first figure (`samples/hello.xp`):

# %%
epix.show("../samples/hello.xp")

# %% [markdown]
# A basic plot (`samples/parabola.xp`) — higher resolution:

# %%
epix.show("../samples/parabola.xp", dpi=200)
