# Novatek nt36xxx Controller Driver for Windows
In this repository you can find the nt36xxx controller driver for Windows (KMDF).
This driver has been only tested with nt36672 controller found in Pocophone F1. For now, it only works with 1080x2246 resolution.

This repository has been forked from https://github.com/theR4K/SynapticsTouch for the base and modified to work with Focal Tech digitizers, and again it was forked from https://github.com/gus33000/FocalTechTouch.
Rest of the README is from the original fork. 

## Disclaimer
This driver is not finished.
It contains debug code and might be missing comments as well.
Tracing has been replaced with KdPrintEx for various reason making development easier on some versions of Windows.

Have fun =)
