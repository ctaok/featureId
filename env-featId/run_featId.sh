#!/bin/bash

DATA_PATH="data.lst"
FEAT_BIN="featureId"
ID_PATH="id_data"

if [ ! -d $ID_PATH ]; then
	mkdir $ID_PATH
fi
./${FEAT_BIN} ${DATA_PATH} ${ID_PATH} feat.map
