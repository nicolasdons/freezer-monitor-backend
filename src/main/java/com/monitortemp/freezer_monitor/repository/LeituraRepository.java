package com.monitortemp.freezer_monitor.repository;

import com.monitortemp.freezer_monitor.model.LeituraTemperatura;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface LeituraRepository extends JpaRepository<LeituraTemperatura, Long> {
}