int main(int argc, char** argv) {
  DataStoreNodeTemplate tlb_stat("tlb_stat"); {
    tlb_stat.summable = 1;
    tlb_stat.addint("hits");
    tlb_stat.addint("misses");
  }
  DataStoreNodeTemplate PerContextOutOfOrderCoreStats("PerContextOutOfOrderCoreStats"); {
    DataStoreNodeTemplate& fetch = PerContextOutOfOrderCoreStats("fetch"); {
      DataStoreNodeTemplate& stop = fetch("stop"); {
        stop.summable = 1;
        stop.addint("stalled");
        stop.addint("icache_miss");
        stop.addint("fetchq_full");
        stop.addint("issueq_quota_full");
        stop.addint("bogus_rip");
        stop.addint("microcode_assist");
        stop.addint("branch_taken");
        stop.addint("full_width");
        stop.addint("icache_stalled");
        stop.addint("invalid_blocks");
      }
      fetch.histogram("opclass", 29, opclass_names);
      fetch.histogram("width", OutOfOrderModel::FETCH_WIDTH+1, 0, OutOfOrderModel::FETCH_WIDTH, 1);
      fetch.addint("blocks");
      fetch.addint("uops");
      fetch.addint("user_insns");
    }
    DataStoreNodeTemplate& frontend = PerContextOutOfOrderCoreStats("frontend"); {
      DataStoreNodeTemplate& status = frontend("status"); {
        status.summable = 1;
        status.addint("complete");
        status.addint("fetchq_empty");
        status.addint("rob_full");
        status.addint("physregs_full");
        status.addint("ldq_full");
        status.addint("stq_full");
      }
      frontend.histogram("width", OutOfOrderModel::FRONTEND_WIDTH+1, 0, OutOfOrderModel::FRONTEND_WIDTH, 1);
      DataStoreNodeTemplate& renamed = frontend("renamed"); {
        renamed.addint("none");
        renamed.addint("reg");
        renamed.addint("flags");
        renamed.addint("reg_and_flags");
      }
      DataStoreNodeTemplate& alloc = frontend("alloc"); {
        alloc.addint("reg");
        alloc.addint("ldreg");
        alloc.addint("sfr");
        alloc.addint("br");
      }
      frontend.histogram("consumer_count", 256, 0, 255, 1);
    }
    DataStoreNodeTemplate& dispatch = PerContextOutOfOrderCoreStats("dispatch"); {
      dispatch.histogram("cluster", OutOfOrderModel::MAX_CLUSTERS, OutOfOrderModel::cluster_names);
      DataStoreNodeTemplate& redispatch = dispatch("redispatch"); {
        redispatch.addint("trigger_uops");
        redispatch.addint("deadlock_flushes");
        redispatch.addint("deadlock_uops_flushed");
        redispatch.histogram("dependent_uops", OutOfOrderModel::ROB_SIZE+1, 0, OutOfOrderModel::ROB_SIZE, 1);
      }
    }
    DataStoreNodeTemplate& issue = PerContextOutOfOrderCoreStats("issue"); {
      issue.addint("uops");
      issue.addfloat("uipc");
      DataStoreNodeTemplate& result = issue("result"); {
        result.summable = 1;
        result.addint("no_fu");
        result.addint("replay");
        result.addint("misspeculated");
        result.addint("refetch");
        result.addint("branch_mispredict");
        result.addint("exception");
        result.addint("complete");
      }
      issue.histogram("opclass", 29, opclass_names);
    }
    DataStoreNodeTemplate& writeback = PerContextOutOfOrderCoreStats("writeback"); {
      writeback.histogram("writebacks", OutOfOrderModel::PHYS_REG_FILE_COUNT, OutOfOrderModel::phys_reg_file_names);
    }
    DataStoreNodeTemplate& commit = PerContextOutOfOrderCoreStats("commit"); {
      commit.addint("uops");
      commit.addint("insns");
      commit.addfloat("uipc");
      commit.addfloat("ipc");
      DataStoreNodeTemplate& result = commit("result"); {
        result.summable = 1;
        result.addint("none");
        result.addint("ok");
        result.addint("exception");
        result.addint("skipblock");
        result.addint("barrier");
        result.addint("smc");
        result.addint("memlocked");
        result.addint("stop");
        result.addint("dcache_stall");
      }
      DataStoreNodeTemplate& fail = commit("fail"); {
        fail.summable = 1;
        fail.addint("free_list");
        fail.addint("frontend_list");
        fail.addint("ready_to_dispatch_list");
        fail.addint("dispatched_list");
        fail.addint("ready_to_issue_list");
        fail.addint("ready_to_store_list");
        fail.addint("ready_to_load_list");
        fail.addint("issued_list");
        fail.addint("completed_list");
        fail.addint("ready_to_writeback_list");
        fail.addint("cache_miss_list");
        fail.addint("tlb_miss_list");
        fail.addint("memory_fence_list");
        fail.addint("ready_to_commit_queue");
      }
      DataStoreNodeTemplate& setflags = commit("setflags"); {
        setflags.summable = 1;
        setflags.addint("yes");
        setflags.addint("no");
      }
      commit.histogram("opclass", 29, opclass_names);
    }
    DataStoreNodeTemplate& branchpred = PerContextOutOfOrderCoreStats("branchpred"); {
      branchpred.addint("predictions");
      branchpred.addint("updates");
      branchpred.histogram("cond", 2, branchpred_outcome_names);
      branchpred.histogram("indir", 2, branchpred_outcome_names);
      branchpred.histogram("ret", 2, branchpred_outcome_names);
      branchpred.histogram("summary", 2, branchpred_outcome_names);
      DataStoreNodeTemplate& ras = branchpred("ras"); {
        ras.summable = 1;
        ras.addint("pushes");
        ras.addint("overflows");
        ras.addint("pops");
        ras.addint("underflows");
        ras.addint("annuls");
      }
    }
    DataStoreNodeTemplate& dcache = PerContextOutOfOrderCoreStats("dcache"); {
      DataStoreNodeTemplate& load = dcache("load"); {
        DataStoreNodeTemplate& issue = load("issue"); {
          issue.summable = 1;
          issue.addint("complete");
          issue.addint("miss");
          issue.addint("exception");
          issue.addint("ordering");
          issue.addint("unaligned");
          DataStoreNodeTemplate& replay = issue("replay"); {
            replay.summable = 1;
            replay.addint("sfr_addr_and_data_not_ready");
            replay.addint("sfr_addr_not_ready");
            replay.addint("sfr_data_not_ready");
            replay.addint("missbuf_full");
            replay.addint("interlocked");
            replay.addint("interlock_overflow");
            replay.addint("fence");
            replay.addint("bank_conflict");
            replay.addint("dcache_stall");
          }
        }
        DataStoreNodeTemplate& forward = load("forward"); {
          forward.summable = 1;
          forward.addint("cache");
          forward.addint("sfr");
          forward.addint("sfr_and_cache");
        }
        DataStoreNodeTemplate& dependency = load("dependency"); {
          dependency.summable = 1;
          dependency.addint("independent");
          dependency.addint("predicted_alias_unresolved");
          dependency.addint("stq_address_match");
          dependency.addint("stq_address_not_ready");
          dependency.addint("fence");
          dependency.addint("mmio");
        }
        DataStoreNodeTemplate& type = load("type"); {
          type.summable = 1;
          type.addint("aligned");
          type.addint("unaligned");
          type.addint("internal");
        }
        load.histogram("size", 4, sizeshift_names);
        load.histogram("datatype", DATATYPE_COUNT, datatype_names);
      }
      DataStoreNodeTemplate& store = dcache("store"); {
        DataStoreNodeTemplate& issue = store("issue"); {
          issue.summable = 1;
          issue.addint("complete");
          issue.addint("exception");
          issue.addint("ordering");
          issue.addint("unaligned");
          DataStoreNodeTemplate& replay = issue("replay"); {
            replay.summable = 1;
            replay.addint("sfr_addr_and_data_not_ready");
            replay.addint("sfr_addr_not_ready");
            replay.addint("sfr_data_not_ready");
            replay.addint("sfr_addr_and_data_and_data_to_store_not_ready");
            replay.addint("sfr_addr_and_data_to_store_not_ready");
            replay.addint("sfr_data_and_data_to_store_not_ready");
            replay.addint("interlocked");
            replay.addint("fence");
            replay.addint("parallel_aliasing");
            replay.addint("bank_conflict");
          }
        }
        DataStoreNodeTemplate& forward = store("forward"); {
          forward.summable = 1;
          forward.addint("zero");
          forward.addint("sfr");
        }
        DataStoreNodeTemplate& type = store("type"); {
          type.summable = 1;
          type.addint("aligned");
          type.addint("unaligned");
          type.addint("internal");
        }
        store.histogram("size", 4, sizeshift_names);
        store.histogram("datatype", DATATYPE_COUNT, datatype_names);
      }
      DataStoreNodeTemplate& fence = dcache("fence"); {
        fence.summable = 1;
        fence.addint("lfence");
        fence.addint("sfence");
        fence.addint("mfence");
      }
      dcache.add("dtlb", tlb_stat);
      dcache.add("itlb", tlb_stat);
      dcache.histogram("dtlb_latency", 1001, 0, 200, 1);
      dcache.histogram("itlb_latency", 1001, 0, 200, 1);
    }
    PerContextOutOfOrderCoreStats.addint("interrupt_requests");
    PerContextOutOfOrderCoreStats.addint("cpu_exit_requests");
    PerContextOutOfOrderCoreStats.addint("cycles_in_pause");
  }
  DataStoreNodeTemplate OutOfOrderCoreStats("OutOfOrderCoreStats"); {
    OutOfOrderCoreStats.addint("cycles");
    DataStoreNodeTemplate& dispatch = OutOfOrderCoreStats("dispatch"); {
      DataStoreNodeTemplate& source = dispatch("source"); {
        source.summable = 1;
        source.histogram("integer", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
        source.histogram("fp", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
        source.histogram("st", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
        source.histogram("br", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
      }
      dispatch.histogram("width", OutOfOrderModel::DISPATCH_WIDTH+1, 0, OutOfOrderModel::DISPATCH_WIDTH, 1);
    }
    DataStoreNodeTemplate& issue = OutOfOrderCoreStats("issue"); {
      DataStoreNodeTemplate& source = issue("source"); {
        source.summable = 1;
        source.histogram("integer", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
        source.histogram("fp", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
        source.histogram("st", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
        source.histogram("br", OutOfOrderModel::MAX_PHYSREG_STATE, OutOfOrderModel::physreg_state_names);
      }
      DataStoreNodeTemplate& width = issue("width"); {
        width.histogram("all", OutOfOrderModel::MAX_ISSUE_WIDTH+1, 0, OutOfOrderModel::MAX_ISSUE_WIDTH, 1);
      }
    }
    DataStoreNodeTemplate& writeback = OutOfOrderCoreStats("writeback"); {
      DataStoreNodeTemplate& width = writeback("width"); {
        width.histogram("all", OutOfOrderModel::MAX_ISSUE_WIDTH+1, 0, OutOfOrderModel::MAX_ISSUE_WIDTH, 1);
      }
    }
    DataStoreNodeTemplate& commit = OutOfOrderCoreStats("commit"); {
      DataStoreNodeTemplate& freereg = commit("freereg"); {
        freereg.summable = 1;
        freereg.addint("pending");
        freereg.addint("free");
      }
      commit.addint("free_regs_recycled");
      commit.histogram("width", OutOfOrderModel::COMMIT_WIDTH+1, 0, OutOfOrderModel::COMMIT_WIDTH, 1);
    }
    DataStoreNodeTemplate& branchpred = OutOfOrderCoreStats("branchpred"); {
      branchpred.addint("predictions");
      branchpred.addint("updates");
      branchpred.histogram("cond", 2, branchpred_outcome_names);
      branchpred.histogram("indir", 2, branchpred_outcome_names);
      branchpred.histogram("ret", 2, branchpred_outcome_names);
      branchpred.histogram("summary", 2, branchpred_outcome_names);
      DataStoreNodeTemplate& ras = branchpred("ras"); {
        ras.summable = 1;
        ras.addint("pushes");
        ras.addint("overflows");
        ras.addint("pops");
        ras.addint("underflows");
        ras.addint("annuls");
      }
    }
    OutOfOrderCoreStats.add("total", PerContextOutOfOrderCoreStats);
    OutOfOrderCoreStats.add("vcpu0", PerContextOutOfOrderCoreStats);
    OutOfOrderCoreStats.add("vcpu1", PerContextOutOfOrderCoreStats);
    OutOfOrderCoreStats.add("vcpu2", PerContextOutOfOrderCoreStats);
    OutOfOrderCoreStats.add("vcpu3", PerContextOutOfOrderCoreStats);
    DataStoreNodeTemplate& simulator = OutOfOrderCoreStats("simulator"); {
      simulator.addfloat("total_time");
      DataStoreNodeTemplate& cputime = simulator("cputime"); {
        cputime.summable = 1;
        cputime.addfloat("fetch");
        cputime.addfloat("decode");
        cputime.addfloat("rename");
        cputime.addfloat("frontend");
        cputime.addfloat("dispatch");
        cputime.addfloat("issue");
        cputime.addfloat("issueload");
        cputime.addfloat("issuestore");
        cputime.addfloat("complete");
        cputime.addfloat("transfer");
        cputime.addfloat("writeback");
        cputime.addfloat("commit");
      }
    }
  }
  DataStoreNodeTemplate PerContextDataCacheStats("PerContextDataCacheStats"); {
    DataStoreNodeTemplate& load = PerContextDataCacheStats("load"); {
      DataStoreNodeTemplate& hit = load("hit"); {
        hit.summable = 1;
        hit.addint("L1");
        hit.addint("L2");
        hit.addint("L3");
        hit.addint("mem");
      }
      DataStoreNodeTemplate& dtlb = load("dtlb"); {
        dtlb.summable = 1;
        dtlb.addint("hits");
        dtlb.addint("misses");
      }
      DataStoreNodeTemplate& tlbwalk = load("tlbwalk"); {
        tlbwalk.summable = 1;
        tlbwalk.addint("L1_dcache_hit");
        tlbwalk.addint("L1_dcache_miss");
        tlbwalk.addint("no_lfrq_mb");
      }
    }
    DataStoreNodeTemplate& fetch = PerContextDataCacheStats("fetch"); {
      DataStoreNodeTemplate& hit = fetch("hit"); {
        hit.summable = 1;
        hit.addint("L1");
        hit.addint("L2");
        hit.addint("L3");
        hit.addint("mem");
      }
      DataStoreNodeTemplate& itlb = fetch("itlb"); {
        itlb.summable = 1;
        itlb.addint("hits");
        itlb.addint("misses");
      }
      DataStoreNodeTemplate& tlbwalk = fetch("tlbwalk"); {
        tlbwalk.summable = 1;
        tlbwalk.addint("L1_dcache_hit");
        tlbwalk.addint("L1_dcache_miss");
        tlbwalk.addint("no_lfrq_mb");
      }
    }
    DataStoreNodeTemplate& store = PerContextDataCacheStats("store"); {
      store.addint("prefetches");
    }
  }
  DataStoreNodeTemplate DataCacheStats("DataCacheStats"); {
    DataStoreNodeTemplate& load = DataCacheStats("load"); {
      DataStoreNodeTemplate& transfer = load("transfer"); {
        transfer.summable = 1;
        transfer.addint("L2_to_L1_full");
        transfer.addint("L2_to_L1_partial");
        transfer.addint("L2_L1I_full");
      }
    }
    DataStoreNodeTemplate& missbuf = DataCacheStats("missbuf"); {
      missbuf.addint("inserts");
      DataStoreNodeTemplate& deliver = missbuf("deliver"); {
        deliver.summable = 1;
        deliver.addint("mem_to_L3");
        deliver.addint("L3_to_L2");
        deliver.addint("L2_to_L1D");
        deliver.addint("L2_to_L1I");
      }
    }
    DataStoreNodeTemplate& prefetch = DataCacheStats("prefetch"); {
      prefetch.summable = 1;
      prefetch.addint("in_L1");
      prefetch.addint("in_L2");
      prefetch.addint("required");
    }
    DataStoreNodeTemplate& lfrq = DataCacheStats("lfrq"); {
      lfrq.addint("inserts");
      lfrq.addint("wakeups");
      lfrq.addint("annuls");
      lfrq.addint("resets");
      lfrq.addint("total_latency");
      lfrq.addfloat("average_latency");
      lfrq.histogram("width", CacheSubsystem::MAX_WAKEUPS_PER_CYCLE+1, 0, CacheSubsystem::MAX_WAKEUPS_PER_CYCLE+1, 1);
    }
    DataCacheStats.add("total", PerContextDataCacheStats);
    DataCacheStats.add("vcpu0", PerContextDataCacheStats);
    DataCacheStats.add("vcpu1", PerContextDataCacheStats);
    DataCacheStats.add("vcpu2", PerContextDataCacheStats);
    DataCacheStats.add("vcpu3", PerContextDataCacheStats);
  }
  DataStoreNodeTemplate stall_sub("stall_sub"); {
    stall_sub.summable = 1;
    stall_sub.addint("dependency");
    stall_sub.addint("cache_port");
    stall_sub.addint("buffer_full");
  }
  DataStoreNodeTemplate CacheStats("CacheStats"); {
    DataStoreNodeTemplate& cpurequest = CacheStats("cpurequest"); {
      DataStoreNodeTemplate& count = cpurequest("count"); {
        count.summable = 1;
        DataStoreNodeTemplate& hit = count("hit"); {
          DataStoreNodeTemplate& read = hit("read"); {
            read.summable = 1;
            DataStoreNodeTemplate& hit = read("hit"); {
              hit.summable = 1;
              hit.addint("hit");
              hit.addint("forward");
            }
          }
          DataStoreNodeTemplate& write = hit("write"); {
            write.summable = 1;
            DataStoreNodeTemplate& hit = write("hit"); {
              hit.summable = 1;
              hit.addint("hit");
              hit.addint("forward");
            }
          }
        }
        DataStoreNodeTemplate& miss = count("miss"); {
          miss.summable = 1;
          miss.addint("read");
          miss.addint("write");
        }
      }
      DataStoreNodeTemplate& stall = cpurequest("stall"); {
        stall.summable = 1;
        stall.add("read", stall_sub);
        stall.add("write", stall_sub);
      }
      cpurequest.addint("redirects");
    }
    DataStoreNodeTemplate& snooprequest = CacheStats("snooprequest"); {
      snooprequest.summable = 1;
      snooprequest.addint("hit");
      snooprequest.addint("miss");
    }
    CacheStats.addint("annul");
    CacheStats.addint("queueFull");
    DataStoreNodeTemplate& latency = CacheStats("latency"); {
      latency.summable = 1;
      latency.addint("IF");
      latency.addint("load");
      latency.addint("store");
    }
    DataStoreNodeTemplate& lat_count = CacheStats("lat_count"); {
      lat_count.summable = 1;
      lat_count.addint("IF");
      lat_count.addint("load");
      lat_count.addint("store");
    }
    DataStoreNodeTemplate& mesi_stats = CacheStats("mesi_stats"); {
      DataStoreNodeTemplate& hit_state = mesi_stats("hit_state"); {
        hit_state.summable = 1;
        hit_state.histogram("snoop", 5, 0, 4, 1);
        hit_state.histogram("cpu", 5, 0, 4, 1);
      }
      mesi_stats.histogram("state_transition", 17, 0, 16, 1);
    }
  }
  DataStoreNodeTemplate PerCoreCacheStats("PerCoreCacheStats"); {
    PerCoreCacheStats.add("CPUController", CacheStats);
    PerCoreCacheStats.add("L1I", CacheStats);
    PerCoreCacheStats.add("L1D", CacheStats);
    PerCoreCacheStats.add("L2", CacheStats);
    PerCoreCacheStats.add("L3", CacheStats);
  }
  DataStoreNodeTemplate BusStats("BusStats"); {
    DataStoreNodeTemplate& broadcasts = BusStats("broadcasts"); {
      broadcasts.summable = 1;
      broadcasts.addint("read");
      broadcasts.addint("write");
      broadcasts.addint("update");
    }
    DataStoreNodeTemplate& broadcast_cycles = BusStats("broadcast_cycles"); {
      broadcast_cycles.summable = 1;
      broadcast_cycles.addint("read");
      broadcast_cycles.addint("write");
      broadcast_cycles.addint("update");
    }
    BusStats.addint("addr_bus_cycles");
    BusStats.addint("data_bus_cycles");
  }
  DataStoreNodeTemplate EventsInMode("EventsInMode"); {
    EventsInMode.summable = 1;
    EventsInMode.addint("user64");
    EventsInMode.addint("user32");
    EventsInMode.addint("kernel64");
    EventsInMode.addint("kernel32");
    EventsInMode.addint("legacy16");
    EventsInMode.addint("userlib");
    EventsInMode.addint("microcode");
    EventsInMode.addint("idle");
  }
  DataStoreNodeTemplate PerCoreEvents("PerCoreEvents"); {
    PerCoreEvents.add("cycles_in_mode", EventsInMode);
    PerCoreEvents.add("insns_in_mode", EventsInMode);
    PerCoreEvents.add("uops_in_mode", EventsInMode);
  }
  DataStoreNodeTemplate PTLsimStats("PTLsimStats"); {
    PTLsimStats.addint("snapshot_uuid");
    PTLsimStats.addstring("snapshot_name", 64);
    DataStoreNodeTemplate& summary = PTLsimStats("summary"); {
      summary.addint("cycles");
      summary.addint("insns");
      summary.addint("uops");
      summary.addint("basicblocks");
    }
    DataStoreNodeTemplate& simulator = PTLsimStats("simulator"); {
      DataStoreNodeTemplate& version = simulator("version"); {
        version.addstring("build_timestamp", 32);
        version.addint("svn_revision");
        version.addstring("svn_timestamp", 32);
        version.addstring("build_hostname", 64);
        version.addstring("build_compiler", 16);
      }
      DataStoreNodeTemplate& run = simulator("run"); {
        run.addint("timestamp");
        run.addstring("hostname", 64);
        run.addstring("kernel_version", 32);
        run.addstring("hypervisor_version", 32);
        run.addint("native_cpuid");
        run.addint("native_hz");
      }
      DataStoreNodeTemplate& config = simulator("config"); {
        config.addstring("config", 256);
      }
      DataStoreNodeTemplate& performance = simulator("performance"); {
        DataStoreNodeTemplate& rate = performance("rate"); {
          rate.addfloat("cycles_per_sec");
          rate.addfloat("issues_per_sec");
          rate.addfloat("user_commits_per_sec");
        }
      }
    }
    DataStoreNodeTemplate& decoder = PTLsimStats("decoder"); {
      DataStoreNodeTemplate& throughput = decoder("throughput"); {
        throughput.addint("basic_blocks");
        throughput.addint("x86_insns");
        throughput.addint("uops");
        throughput.addint("bytes");
      }
      decoder.histogram("x86_decode_type", DECODE_TYPE_COUNT, decode_type_names);
      DataStoreNodeTemplate& bb_decode_type = decoder("bb_decode_type"); {
        bb_decode_type.summable = 1;
        bb_decode_type.addint("all_insns_fast");
        bb_decode_type.addint("some_complex_insns");
      }
      DataStoreNodeTemplate& page_crossings = decoder("page_crossings"); {
        page_crossings.summable = 1;
        page_crossings.addint("within_page");
        page_crossings.addint("crosses_page");
      }
      DataStoreNodeTemplate& bbcache = decoder("bbcache"); {
        bbcache.addint("count");
        bbcache.addint("inserts");
        bbcache.histogram("invalidates", INVALIDATE_REASON_COUNT, invalidate_reason_names);
      }
      DataStoreNodeTemplate& pagecache = decoder("pagecache"); {
        pagecache.addint("count");
        pagecache.addint("inserts");
        pagecache.histogram("invalidates", INVALIDATE_REASON_COUNT, invalidate_reason_names);
      }
      decoder.addint("reclaim_rounds");
    }
    PTLsimStats.add("ooocore_total", OutOfOrderCoreStats);
    PTLsimStats.add("ooocore_context_total", PerContextOutOfOrderCoreStats);
    PTLsimStats.addint("elapse_seconds");
    DataStoreNodeTemplate& ooocore = PTLsimStats("ooocore"); {
      ooocore.add("total", OutOfOrderCoreStats);
      ooocore.add("c0", OutOfOrderCoreStats);
      ooocore.add("c1", OutOfOrderCoreStats);
      ooocore.add("c2", OutOfOrderCoreStats);
      ooocore.add("c3", OutOfOrderCoreStats);
      ooocore.add("c4", OutOfOrderCoreStats);
      ooocore.add("c5", OutOfOrderCoreStats);
      ooocore.add("c6", OutOfOrderCoreStats);
      ooocore.add("c7", OutOfOrderCoreStats);
    }
    DataStoreNodeTemplate& dcache = PTLsimStats("dcache"); {
      dcache.add("total", DataCacheStats);
      dcache.add("c0", DataCacheStats);
      dcache.add("c1", DataCacheStats);
      dcache.add("c2", DataCacheStats);
      dcache.add("c3", DataCacheStats);
      dcache.add("c4", DataCacheStats);
      dcache.add("c5", DataCacheStats);
      dcache.add("c6", DataCacheStats);
      dcache.add("c7", DataCacheStats);
    }
    DataStoreNodeTemplate& external = PTLsimStats("external"); {
      external.histogram("assists", ASSIST_COUNT, assist_names);
      external.histogram("l_assists", L_ASSIST_COUNT, light_assist_names);
      external.histogram("traps", 256, x86_exception_names);
      external.add("total", PerCoreEvents);
      external.add("c0", PerCoreEvents);
      external.add("c1", PerCoreEvents);
      external.add("c2", PerCoreEvents);
      external.add("c3", PerCoreEvents);
      external.add("c4", PerCoreEvents);
      external.add("c5", PerCoreEvents);
      external.add("c6", PerCoreEvents);
      external.add("c7", PerCoreEvents);
    }
    DataStoreNodeTemplate& memory = PTLsimStats("memory"); {
      memory.add("total", PerCoreCacheStats);
      memory.add("c0", PerCoreCacheStats);
      memory.add("c1", PerCoreCacheStats);
      memory.add("c2", PerCoreCacheStats);
      memory.add("c3", PerCoreCacheStats);
      memory.add("c4", PerCoreCacheStats);
      memory.add("c5", PerCoreCacheStats);
      memory.add("c6", PerCoreCacheStats);
      memory.add("c7", PerCoreCacheStats);
      memory.add("bus", BusStats);
      memory.histogram("dcache_latency", 200, 0, 199, 1);
      memory.histogram("icache_latency", 200, 0, 199, 1);
    }
  }
  ofstream os(argv[1], std::ios::binary |std::ios::out);
  PTLsimStats.write(os);
  os.close();
}
