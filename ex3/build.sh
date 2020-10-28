#!/bin/bash

ptxdist select configs/ptxconfig

ptxdist platform configs/platform-energymicro-efm32gg-dk3750/platformconfig

ptxdist images && ptxdist test flash-all
