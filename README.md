# MODL
modl is a modular programming language

## Why?

First of all, why not? I know there's going to be people saying "Why not just use X, it has everything you're describing", but that's not the point.
I'm just experimenting and learning something new.

## What's special about modl?

Some background.
I recently became obsessed with MTP (Meta Template Programming) in C++, I would say I use them more than I should...

My biggest flaws with C++ is that writing in MTP isn't like writing standard C++. For example, managing parameter packs is just a pain.
 everything has to be done recursively, which can be hard to 
There lies the goal of modl. Compile time configurations are written in the exact same way that the rest of the language is.

A big inspiration to this language is Rust with their macros and their explicit ownership model

## Design

The core design of the language is still a WIP
I know I want it to be C style, and will share a lot of the same features as C++, but that's it so far.

## Usage

```
git clone https://github.com/conradludgate/modl
cd modl
make
./modl tests/*
```