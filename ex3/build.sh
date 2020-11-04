#!/bin/bash

ptxdist select configs/ptxconfig

ptxdist platform configs/platform-energymicro-efm32gg-dk3750/platformconfig

ptxdist clean driver-gamepad
ptxdist clean game
ptxdist images && ptxdist test flash-all
