package com.monitortemp.freezer_monitor.controller;

import com.monitortemp.freezer_monitor.dto.LeituraDTO;
import com.monitortemp.freezer_monitor.model.LeituraTemperatura;
import com.monitortemp.freezer_monitor.repository.LeituraRepository;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/temperatura")
public class TemperaturaController {

    private final LeituraRepository repository;

    public TemperaturaController(LeituraRepository repository) {
        this.repository = repository;
    }

    @PostMapping
    public ResponseEntity<Void> receberLeitura(@RequestBody LeituraDTO dto) {
        System.out.println("LOG: Recebido de " + dto.macAddress() + " -> " + dto.temperatura() + " °C");

        LeituraTemperatura leitura = new LeituraTemperatura(dto.macAddress(), dto.temperatura());
        repository.save(leitura);

        return ResponseEntity.ok().build();
    }

    @GetMapping
    public ResponseEntity<List<LeituraTemperatura>> listarLeituras() {
        return ResponseEntity.ok(repository.findAll());
    }
}