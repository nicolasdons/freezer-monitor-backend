package com.monitortemp.freezer_monitor.dto;

public record LeituraDTO(
        String macAddress,
        Double temperatura
) {}