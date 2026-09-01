package com.monitortemp.freezer_monitor.model;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "tb_leituras")
public class LeituraTemperatura {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String macAddress;
    private Double temperatura;
    private LocalDateTime dataHora;

    public LeituraTemperatura() {}

    public LeituraTemperatura(String macAddress, Double temperatura) {
        this.macAddress = macAddress;
        this.temperatura = temperatura;
        this.dataHora = LocalDateTime.now();
    }

    public Long getId() { return id; }
    public String getMacAddress() { return macAddress; }
    public Double getTemperatura() { return temperatura; }
    public LocalDateTime getDataHora() { return dataHora; }

    public void setMacAddress(String macAddress) { this.macAddress = macAddress; }
    public void setTemperatura(Double temperatura) { this.temperatura = temperatura; }
    public void setDataHora(LocalDateTime dataHora) { this.dataHora = dataHora; }
}