package com.monitortemp.freezer_monitor.dto;

import com.fasterxml.jackson.annotation.JsonProperty;

public record LeituraDTO(
        @JsonProperty("macAddress") String macAddress,
        @JsonProperty("temperatura") Double temperatura
) {}