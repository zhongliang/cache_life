#include <bson/bson.h>

void add_bson_obj_attr_bool(bson_buffer *bb, const char *key, int value) {
  bson_append_bool(bb, key, (bson_bool_t)(value));
  return;
}

void add_bson_tlb_stat(tlb_stat *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);
  add_bson_obj_attr_bool(obj, "summable", 1);

  bson_append_long(bb, "hits", stats->hits);
  bson_append_long(bb, "misses", stats->misses);
  bson_append_finish_object(bb);
}

void add_bson_PerContextOutOfOrderCoreStats(PerContextOutOfOrderCoreStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  {
    const char *node_name = "fetch";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "stop";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "stalled", stats->fetch.stop.stalled);
      bson_append_long(bb, "icache_miss", stats->fetch.stop.icache_miss);
      bson_append_long(bb, "fetchq_full", stats->fetch.stop.fetchq_full);
      bson_append_long(bb, "issueq_quota_full", stats->fetch.stop.issueq_quota_full);
      bson_append_long(bb, "bogus_rip", stats->fetch.stop.bogus_rip);
      bson_append_long(bb, "microcode_assist", stats->fetch.stop.microcode_assist);
      bson_append_long(bb, "branch_taken", stats->fetch.stop.branch_taken);
      bson_append_long(bb, "full_width", stats->fetch.stop.full_width);
      bson_append_long(bb, "icache_stalled", stats->fetch.stop.icache_stalled);
      bson_append_long(bb, "invalid_blocks", stats->fetch.stop.invalid_blocks);
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "opclass");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 29; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, opclass_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 29; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->fetch.opclass[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "width");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < OutOfOrderModel::FETCH_WIDTH+1; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->fetch.width[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_long(bb, "blocks", stats->fetch.blocks);
    bson_append_long(bb, "uops", stats->fetch.uops);
    bson_append_long(bb, "user_insns", stats->fetch.user_insns);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "frontend";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "status";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "complete", stats->frontend.status.complete);
      bson_append_long(bb, "fetchq_empty", stats->frontend.status.fetchq_empty);
      bson_append_long(bb, "rob_full", stats->frontend.status.rob_full);
      bson_append_long(bb, "physregs_full", stats->frontend.status.physregs_full);
      bson_append_long(bb, "ldq_full", stats->frontend.status.ldq_full);
      bson_append_long(bb, "stq_full", stats->frontend.status.stq_full);
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "width");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < OutOfOrderModel::FRONTEND_WIDTH+1; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->frontend.width[i]);
      }
      bson_append_finish_object(arr);
    }

    {
      const char *node_name = "renamed";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "none", stats->frontend.renamed.none);
      bson_append_long(bb, "reg", stats->frontend.renamed.reg);
      bson_append_long(bb, "flags", stats->frontend.renamed.flags);
      bson_append_long(bb, "reg_and_flags", stats->frontend.renamed.reg_and_flags);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "alloc";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "reg", stats->frontend.alloc.reg);
      bson_append_long(bb, "ldreg", stats->frontend.alloc.ldreg);
      bson_append_long(bb, "sfr", stats->frontend.alloc.sfr);
      bson_append_long(bb, "br", stats->frontend.alloc.br);
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "consumer_count");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < 256; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->frontend.consumer_count[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "dispatch";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "cluster");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < OutOfOrderModel::MAX_CLUSTERS; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, OutOfOrderModel::cluster_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < OutOfOrderModel::MAX_CLUSTERS; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->dispatch.cluster[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      const char *node_name = "redispatch";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "trigger_uops", stats->dispatch.redispatch.trigger_uops);
      bson_append_long(bb, "deadlock_flushes", stats->dispatch.redispatch.deadlock_flushes);
      bson_append_long(bb, "deadlock_uops_flushed", stats->dispatch.redispatch.deadlock_uops_flushed);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "dependent_uops");
        bson_append_bool(bb, "histogram", (bson_bool_t)1);
        for(int i = 0; i < OutOfOrderModel::ROB_SIZE+1; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dispatch.redispatch.dependent_uops[i]);
        }
        bson_append_finish_object(arr);
      }

      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "issue";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "uops", stats->issue.uops);
    bson_append_long(bb, "uipc", stats->issue.uipc);
    {
      const char *node_name = "result";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "no_fu", stats->issue.result.no_fu);
      bson_append_long(bb, "replay", stats->issue.result.replay);
      bson_append_long(bb, "misspeculated", stats->issue.result.misspeculated);
      bson_append_long(bb, "refetch", stats->issue.result.refetch);
      bson_append_long(bb, "branch_mispredict", stats->issue.result.branch_mispredict);
      bson_append_long(bb, "exception", stats->issue.result.exception);
      bson_append_long(bb, "complete", stats->issue.result.complete);
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "opclass");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 29; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, opclass_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 29; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->issue.opclass[i]);
      }
      bson_append_finish_object(arr);
    }
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "writeback";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "writebacks");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < OutOfOrderModel::PHYS_REG_FILE_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, OutOfOrderModel::phys_reg_file_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < OutOfOrderModel::PHYS_REG_FILE_COUNT; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->writeback.writebacks[i]);
      }
      bson_append_finish_object(arr);
    }
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "commit";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "uops", stats->commit.uops);
    bson_append_long(bb, "insns", stats->commit.insns);
    bson_append_long(bb, "uipc", stats->commit.uipc);
    bson_append_long(bb, "ipc", stats->commit.ipc);
    {
      const char *node_name = "result";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "none", stats->commit.result.none);
      bson_append_long(bb, "ok", stats->commit.result.ok);
      bson_append_long(bb, "exception", stats->commit.result.exception);
      bson_append_long(bb, "skipblock", stats->commit.result.skipblock);
      bson_append_long(bb, "barrier", stats->commit.result.barrier);
      bson_append_long(bb, "smc", stats->commit.result.smc);
      bson_append_long(bb, "memlocked", stats->commit.result.memlocked);
      bson_append_long(bb, "stop", stats->commit.result.stop);
      bson_append_long(bb, "dcache_stall", stats->commit.result.dcache_stall);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "fail";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "free_list", stats->commit.fail.free_list);
      bson_append_long(bb, "frontend_list", stats->commit.fail.frontend_list);
      bson_append_long(bb, "ready_to_dispatch_list", stats->commit.fail.ready_to_dispatch_list);
      bson_append_long(bb, "dispatched_list", stats->commit.fail.dispatched_list);
      bson_append_long(bb, "ready_to_issue_list", stats->commit.fail.ready_to_issue_list);
      bson_append_long(bb, "ready_to_store_list", stats->commit.fail.ready_to_store_list);
      bson_append_long(bb, "ready_to_load_list", stats->commit.fail.ready_to_load_list);
      bson_append_long(bb, "issued_list", stats->commit.fail.issued_list);
      bson_append_long(bb, "completed_list", stats->commit.fail.completed_list);
      bson_append_long(bb, "ready_to_writeback_list", stats->commit.fail.ready_to_writeback_list);
      bson_append_long(bb, "cache_miss_list", stats->commit.fail.cache_miss_list);
      bson_append_long(bb, "tlb_miss_list", stats->commit.fail.tlb_miss_list);
      bson_append_long(bb, "memory_fence_list", stats->commit.fail.memory_fence_list);
      bson_append_long(bb, "ready_to_commit_queue", stats->commit.fail.ready_to_commit_queue);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "setflags";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "yes", stats->commit.setflags.yes);
      bson_append_long(bb, "no", stats->commit.setflags.no);
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "opclass");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 29; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, opclass_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 29; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->commit.opclass[i]);
      }
      bson_append_finish_object(arr);
    }
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "branchpred";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "predictions", stats->branchpred.predictions);
    bson_append_long(bb, "updates", stats->branchpred.updates);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "cond");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.cond[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "indir");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.indir[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "ret");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.ret[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "summary");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.summary[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      const char *node_name = "ras";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "pushes", stats->branchpred.ras.pushes);
      bson_append_long(bb, "overflows", stats->branchpred.ras.overflows);
      bson_append_long(bb, "pops", stats->branchpred.ras.pops);
      bson_append_long(bb, "underflows", stats->branchpred.ras.underflows);
      bson_append_long(bb, "annuls", stats->branchpred.ras.annuls);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "dcache";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "load";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      {
        const char *node_name = "issue";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "complete", stats->dcache.load.issue.complete);
        bson_append_long(bb, "miss", stats->dcache.load.issue.miss);
        bson_append_long(bb, "exception", stats->dcache.load.issue.exception);
        bson_append_long(bb, "ordering", stats->dcache.load.issue.ordering);
        bson_append_long(bb, "unaligned", stats->dcache.load.issue.unaligned);
        {
          const char *node_name = "replay";
          bson_buffer *obj = bson_append_start_object(bb, node_name);
          add_bson_obj_attr_bool(obj, "summable", 1);
          bson_append_long(bb, "sfr_addr_and_data_not_ready", stats->dcache.load.issue.replay.sfr_addr_and_data_not_ready);
          bson_append_long(bb, "sfr_addr_not_ready", stats->dcache.load.issue.replay.sfr_addr_not_ready);
          bson_append_long(bb, "sfr_data_not_ready", stats->dcache.load.issue.replay.sfr_data_not_ready);
          bson_append_long(bb, "missbuf_full", stats->dcache.load.issue.replay.missbuf_full);
          bson_append_long(bb, "interlocked", stats->dcache.load.issue.replay.interlocked);
          bson_append_long(bb, "interlock_overflow", stats->dcache.load.issue.replay.interlock_overflow);
          bson_append_long(bb, "fence", stats->dcache.load.issue.replay.fence);
          bson_append_long(bb, "bank_conflict", stats->dcache.load.issue.replay.bank_conflict);
          bson_append_long(bb, "dcache_stall", stats->dcache.load.issue.replay.dcache_stall);
          bson_append_finish_object(bb);
        }

        bson_append_finish_object(bb);
      }

      {
        const char *node_name = "forward";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "cache", stats->dcache.load.forward.cache);
        bson_append_long(bb, "sfr", stats->dcache.load.forward.sfr);
        bson_append_long(bb, "sfr_and_cache", stats->dcache.load.forward.sfr_and_cache);
        bson_append_finish_object(bb);
      }

      {
        const char *node_name = "dependency";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "independent", stats->dcache.load.dependency.independent);
        bson_append_long(bb, "predicted_alias_unresolved", stats->dcache.load.dependency.predicted_alias_unresolved);
        bson_append_long(bb, "stq_address_match", stats->dcache.load.dependency.stq_address_match);
        bson_append_long(bb, "stq_address_not_ready", stats->dcache.load.dependency.stq_address_not_ready);
        bson_append_long(bb, "fence", stats->dcache.load.dependency.fence);
        bson_append_long(bb, "mmio", stats->dcache.load.dependency.mmio);
        bson_append_finish_object(bb);
      }

      {
        const char *node_name = "type";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "aligned", stats->dcache.load.type.aligned);
        bson_append_long(bb, "unaligned", stats->dcache.load.type.unaligned);
        bson_append_long(bb, "internal", stats->dcache.load.type.internal);
        bson_append_finish_object(bb);
      }

      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "size");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < 4; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, sizeshift_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < 4; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dcache.load.size[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "datatype");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < DATATYPE_COUNT; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, datatype_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < DATATYPE_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dcache.load.datatype[i]);
        }
        bson_append_finish_object(arr);
      }
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "store";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      {
        const char *node_name = "issue";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "complete", stats->dcache.store.issue.complete);
        bson_append_long(bb, "exception", stats->dcache.store.issue.exception);
        bson_append_long(bb, "ordering", stats->dcache.store.issue.ordering);
        bson_append_long(bb, "unaligned", stats->dcache.store.issue.unaligned);
        {
          const char *node_name = "replay";
          bson_buffer *obj = bson_append_start_object(bb, node_name);
          add_bson_obj_attr_bool(obj, "summable", 1);
          bson_append_long(bb, "sfr_addr_and_data_not_ready", stats->dcache.store.issue.replay.sfr_addr_and_data_not_ready);
          bson_append_long(bb, "sfr_addr_not_ready", stats->dcache.store.issue.replay.sfr_addr_not_ready);
          bson_append_long(bb, "sfr_data_not_ready", stats->dcache.store.issue.replay.sfr_data_not_ready);
          bson_append_long(bb, "sfr_addr_and_data_and_data_to_store_not_ready", stats->dcache.store.issue.replay.sfr_addr_and_data_and_data_to_store_not_ready);
          bson_append_long(bb, "sfr_addr_and_data_to_store_not_ready", stats->dcache.store.issue.replay.sfr_addr_and_data_to_store_not_ready);
          bson_append_long(bb, "sfr_data_and_data_to_store_not_ready", stats->dcache.store.issue.replay.sfr_data_and_data_to_store_not_ready);
          bson_append_long(bb, "interlocked", stats->dcache.store.issue.replay.interlocked);
          bson_append_long(bb, "fence", stats->dcache.store.issue.replay.fence);
          bson_append_long(bb, "parallel_aliasing", stats->dcache.store.issue.replay.parallel_aliasing);
          bson_append_long(bb, "bank_conflict", stats->dcache.store.issue.replay.bank_conflict);
          bson_append_finish_object(bb);
        }

        bson_append_finish_object(bb);
      }

      {
        const char *node_name = "forward";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "zero", stats->dcache.store.forward.zero);
        bson_append_long(bb, "sfr", stats->dcache.store.forward.sfr);
        bson_append_finish_object(bb);
      }

      {
        const char *node_name = "type";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "aligned", stats->dcache.store.type.aligned);
        bson_append_long(bb, "unaligned", stats->dcache.store.type.unaligned);
        bson_append_long(bb, "internal", stats->dcache.store.type.internal);
        bson_append_finish_object(bb);
      }

      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "size");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < 4; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, sizeshift_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < 4; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dcache.store.size[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "datatype");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < DATATYPE_COUNT; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, datatype_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < DATATYPE_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dcache.store.datatype[i]);
        }
        bson_append_finish_object(arr);
      }
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "fence";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "lfence", stats->dcache.fence.lfence);
      bson_append_long(bb, "sfence", stats->dcache.fence.sfence);
      bson_append_long(bb, "mfence", stats->dcache.fence.mfence);
      bson_append_finish_object(bb);
    }

    add_bson_tlb_stat(&stats->dcache.dtlb, bb, "dtlb");
    add_bson_tlb_stat(&stats->dcache.itlb, bb, "itlb");
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "dtlb_latency");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < 1001; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->dcache.dtlb_latency[i]);
      }
      bson_append_finish_object(arr);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "itlb_latency");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < 1001; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->dcache.itlb_latency[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  bson_append_long(bb, "interrupt_requests", stats->interrupt_requests);
  bson_append_long(bb, "cpu_exit_requests", stats->cpu_exit_requests);
  bson_append_long(bb, "cycles_in_pause", stats->cycles_in_pause);
  bson_append_finish_object(bb);
}

void add_bson_OutOfOrderCoreStats(OutOfOrderCoreStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  bson_append_long(bb, "cycles", stats->cycles);
  {
    const char *node_name = "dispatch";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "source";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "integer");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dispatch.source.integer[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "fp");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dispatch.source.fp[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "st");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dispatch.source.st[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "br");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->dispatch.source.br[i]);
        }
        bson_append_finish_object(arr);
      }
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "width");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < OutOfOrderModel::DISPATCH_WIDTH+1; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->dispatch.width[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "issue";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "source";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "integer");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->issue.source.integer[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "fp");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->issue.source.fp[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "st");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->issue.source.st[i]);
        }
        bson_append_finish_object(arr);
      }
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "br");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, OutOfOrderModel::physreg_state_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < OutOfOrderModel::MAX_PHYSREG_STATE; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->issue.source.br[i]);
        }
        bson_append_finish_object(arr);
      }
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "width";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "all");
        bson_append_bool(bb, "histogram", (bson_bool_t)1);
        for(int i = 0; i < OutOfOrderModel::MAX_ISSUE_WIDTH+1; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->issue.width.all[i]);
        }
        bson_append_finish_object(arr);
      }

      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "writeback";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "width";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "all");
        bson_append_bool(bb, "histogram", (bson_bool_t)1);
        for(int i = 0; i < OutOfOrderModel::MAX_ISSUE_WIDTH+1; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->writeback.width.all[i]);
        }
        bson_append_finish_object(arr);
      }

      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "commit";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "freereg";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "pending", stats->commit.freereg.pending);
      bson_append_long(bb, "free", stats->commit.freereg.free);
      bson_append_finish_object(bb);
    }

    bson_append_long(bb, "free_regs_recycled", stats->commit.free_regs_recycled);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "width");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < OutOfOrderModel::COMMIT_WIDTH+1; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->commit.width[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "branchpred";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "predictions", stats->branchpred.predictions);
    bson_append_long(bb, "updates", stats->branchpred.updates);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "cond");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.cond[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "indir");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.indir[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "ret");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.ret[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "summary");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 2; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, branchpred_outcome_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 2; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->branchpred.summary[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      const char *node_name = "ras";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "pushes", stats->branchpred.ras.pushes);
      bson_append_long(bb, "overflows", stats->branchpred.ras.overflows);
      bson_append_long(bb, "pops", stats->branchpred.ras.pops);
      bson_append_long(bb, "underflows", stats->branchpred.ras.underflows);
      bson_append_long(bb, "annuls", stats->branchpred.ras.annuls);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  add_bson_PerContextOutOfOrderCoreStats(&stats->total, bb, "total");
  add_bson_PerContextOutOfOrderCoreStats(&stats->vcpu0, bb, "vcpu0");
  add_bson_PerContextOutOfOrderCoreStats(&stats->vcpu1, bb, "vcpu1");
  add_bson_PerContextOutOfOrderCoreStats(&stats->vcpu2, bb, "vcpu2");
  add_bson_PerContextOutOfOrderCoreStats(&stats->vcpu3, bb, "vcpu3");
  {
    const char *node_name = "simulator";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "total_time", stats->simulator.total_time);
    {
      const char *node_name = "cputime";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "fetch", stats->simulator.cputime.fetch);
      bson_append_long(bb, "decode", stats->simulator.cputime.decode);
      bson_append_long(bb, "rename", stats->simulator.cputime.rename);
      bson_append_long(bb, "frontend", stats->simulator.cputime.frontend);
      bson_append_long(bb, "dispatch", stats->simulator.cputime.dispatch);
      bson_append_long(bb, "issue", stats->simulator.cputime.issue);
      bson_append_long(bb, "issueload", stats->simulator.cputime.issueload);
      bson_append_long(bb, "issuestore", stats->simulator.cputime.issuestore);
      bson_append_long(bb, "complete", stats->simulator.cputime.complete);
      bson_append_long(bb, "transfer", stats->simulator.cputime.transfer);
      bson_append_long(bb, "writeback", stats->simulator.cputime.writeback);
      bson_append_long(bb, "commit", stats->simulator.cputime.commit);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  bson_append_finish_object(bb);
}

void add_bson_PerContextDataCacheStats(PerContextDataCacheStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  {
    const char *node_name = "load";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "hit";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "L1", stats->load.hit.L1);
      bson_append_long(bb, "L2", stats->load.hit.L2);
      bson_append_long(bb, "L3", stats->load.hit.L3);
      bson_append_long(bb, "mem", stats->load.hit.mem);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "dtlb";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "hits", stats->load.dtlb.hits);
      bson_append_long(bb, "misses", stats->load.dtlb.misses);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "tlbwalk";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "L1_dcache_hit", stats->load.tlbwalk.L1_dcache_hit);
      bson_append_long(bb, "L1_dcache_miss", stats->load.tlbwalk.L1_dcache_miss);
      bson_append_long(bb, "no_lfrq_mb", stats->load.tlbwalk.no_lfrq_mb);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "fetch";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "hit";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "L1", stats->fetch.hit.L1);
      bson_append_long(bb, "L2", stats->fetch.hit.L2);
      bson_append_long(bb, "L3", stats->fetch.hit.L3);
      bson_append_long(bb, "mem", stats->fetch.hit.mem);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "itlb";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "hits", stats->fetch.itlb.hits);
      bson_append_long(bb, "misses", stats->fetch.itlb.misses);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "tlbwalk";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "L1_dcache_hit", stats->fetch.tlbwalk.L1_dcache_hit);
      bson_append_long(bb, "L1_dcache_miss", stats->fetch.tlbwalk.L1_dcache_miss);
      bson_append_long(bb, "no_lfrq_mb", stats->fetch.tlbwalk.no_lfrq_mb);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "store";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "prefetches", stats->store.prefetches);
    bson_append_finish_object(bb);
  }

  bson_append_finish_object(bb);
}

void add_bson_DataCacheStats(DataCacheStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  {
    const char *node_name = "load";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "transfer";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "L2_to_L1_full", stats->load.transfer.L2_to_L1_full);
      bson_append_long(bb, "L2_to_L1_partial", stats->load.transfer.L2_to_L1_partial);
      bson_append_long(bb, "L2_L1I_full", stats->load.transfer.L2_L1I_full);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "missbuf";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "inserts", stats->missbuf.inserts);
    {
      const char *node_name = "deliver";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "mem_to_L3", stats->missbuf.deliver.mem_to_L3);
      bson_append_long(bb, "L3_to_L2", stats->missbuf.deliver.L3_to_L2);
      bson_append_long(bb, "L2_to_L1D", stats->missbuf.deliver.L2_to_L1D);
      bson_append_long(bb, "L2_to_L1I", stats->missbuf.deliver.L2_to_L1I);
      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "prefetch";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_obj_attr_bool(obj, "summable", 1);
    bson_append_long(bb, "in_L1", stats->prefetch.in_L1);
    bson_append_long(bb, "in_L2", stats->prefetch.in_L2);
    bson_append_long(bb, "required", stats->prefetch.required);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "lfrq";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "inserts", stats->lfrq.inserts);
    bson_append_long(bb, "wakeups", stats->lfrq.wakeups);
    bson_append_long(bb, "annuls", stats->lfrq.annuls);
    bson_append_long(bb, "resets", stats->lfrq.resets);
    bson_append_long(bb, "total_latency", stats->lfrq.total_latency);
    bson_append_long(bb, "average_latency", stats->lfrq.average_latency);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "width");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < CacheSubsystem::MAX_WAKEUPS_PER_CYCLE+1; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->lfrq.width[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  add_bson_PerContextDataCacheStats(&stats->total, bb, "total");
  add_bson_PerContextDataCacheStats(&stats->vcpu0, bb, "vcpu0");
  add_bson_PerContextDataCacheStats(&stats->vcpu1, bb, "vcpu1");
  add_bson_PerContextDataCacheStats(&stats->vcpu2, bb, "vcpu2");
  add_bson_PerContextDataCacheStats(&stats->vcpu3, bb, "vcpu3");
  bson_append_finish_object(bb);
}

void add_bson_stall_sub(stall_sub *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);
  add_bson_obj_attr_bool(obj, "summable", 1);

  bson_append_long(bb, "dependency", stats->dependency);
  bson_append_long(bb, "cache_port", stats->cache_port);
  bson_append_long(bb, "buffer_full", stats->buffer_full);
  bson_append_finish_object(bb);
}

void add_bson_CacheStats(CacheStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  {
    const char *node_name = "cpurequest";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "count";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      {
        const char *node_name = "hit";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        {
          const char *node_name = "read";
          bson_buffer *obj = bson_append_start_object(bb, node_name);
          add_bson_obj_attr_bool(obj, "summable", 1);
          {
            const char *node_name = "hit";
            bson_buffer *obj = bson_append_start_object(bb, node_name);
            add_bson_obj_attr_bool(obj, "summable", 1);
            bson_append_long(bb, "hit", stats->cpurequest.count.hit.read.hit.hit);
            bson_append_long(bb, "forward", stats->cpurequest.count.hit.read.hit.forward);
            bson_append_finish_object(bb);
          }

          bson_append_finish_object(bb);
        }

        {
          const char *node_name = "write";
          bson_buffer *obj = bson_append_start_object(bb, node_name);
          add_bson_obj_attr_bool(obj, "summable", 1);
          {
            const char *node_name = "hit";
            bson_buffer *obj = bson_append_start_object(bb, node_name);
            add_bson_obj_attr_bool(obj, "summable", 1);
            bson_append_long(bb, "hit", stats->cpurequest.count.hit.write.hit.hit);
            bson_append_long(bb, "forward", stats->cpurequest.count.hit.write.hit.forward);
            bson_append_finish_object(bb);
          }

          bson_append_finish_object(bb);
        }

        bson_append_finish_object(bb);
      }

      {
        const char *node_name = "miss";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        add_bson_obj_attr_bool(obj, "summable", 1);
        bson_append_long(bb, "read", stats->cpurequest.count.miss.read);
        bson_append_long(bb, "write", stats->cpurequest.count.miss.write);
        bson_append_finish_object(bb);
      }

      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "stall";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      add_bson_stall_sub(&stats->cpurequest.stall.read, bb, "read");
      add_bson_stall_sub(&stats->cpurequest.stall.write, bb, "write");
      bson_append_finish_object(bb);
    }

    bson_append_long(bb, "redirects", stats->cpurequest.redirects);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "snooprequest";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_obj_attr_bool(obj, "summable", 1);
    bson_append_long(bb, "hit", stats->snooprequest.hit);
    bson_append_long(bb, "miss", stats->snooprequest.miss);
    bson_append_finish_object(bb);
  }

  bson_append_long(bb, "annul", stats->annul);
  bson_append_long(bb, "queueFull", stats->queueFull);
  {
    const char *node_name = "latency";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_obj_attr_bool(obj, "summable", 1);
    bson_append_long(bb, "IF", stats->latency.IF);
    bson_append_long(bb, "load", stats->latency.load);
    bson_append_long(bb, "store", stats->latency.store);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "lat_count";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_obj_attr_bool(obj, "summable", 1);
    bson_append_long(bb, "IF", stats->lat_count.IF);
    bson_append_long(bb, "load", stats->lat_count.load);
    bson_append_long(bb, "store", stats->lat_count.store);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "mesi_stats";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "hit_state";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "snoop");
        bson_append_bool(bb, "histogram", (bson_bool_t)1);
        for(int i = 0; i < 5; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->mesi_stats.hit_state.snoop[i]);
        }
        bson_append_finish_object(arr);
      }

      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "cpu");
        bson_append_bool(bb, "histogram", (bson_bool_t)1);
        for(int i = 0; i < 5; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->mesi_stats.hit_state.cpu[i]);
        }
        bson_append_finish_object(arr);
      }

      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "state_transition");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < 17; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->mesi_stats.state_transition[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  bson_append_finish_object(bb);
}

void add_bson_PerCoreCacheStats(PerCoreCacheStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  add_bson_CacheStats(&stats->CPUController, bb, "CPUController");
  add_bson_CacheStats(&stats->L1I, bb, "L1I");
  add_bson_CacheStats(&stats->L1D, bb, "L1D");
  add_bson_CacheStats(&stats->L2, bb, "L2");
  add_bson_CacheStats(&stats->L3, bb, "L3");
  bson_append_finish_object(bb);
}

void add_bson_BusStats(BusStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  {
    const char *node_name = "broadcasts";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_obj_attr_bool(obj, "summable", 1);
    bson_append_long(bb, "read", stats->broadcasts.read);
    bson_append_long(bb, "write", stats->broadcasts.write);
    bson_append_long(bb, "update", stats->broadcasts.update);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "broadcast_cycles";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_obj_attr_bool(obj, "summable", 1);
    bson_append_long(bb, "read", stats->broadcast_cycles.read);
    bson_append_long(bb, "write", stats->broadcast_cycles.write);
    bson_append_long(bb, "update", stats->broadcast_cycles.update);
    bson_append_finish_object(bb);
  }

  bson_append_long(bb, "addr_bus_cycles", stats->addr_bus_cycles);
  bson_append_long(bb, "data_bus_cycles", stats->data_bus_cycles);
  bson_append_finish_object(bb);
}

void add_bson_EventsInMode(EventsInMode *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);
  add_bson_obj_attr_bool(obj, "summable", 1);

  bson_append_long(bb, "user64", stats->user64);
  bson_append_long(bb, "user32", stats->user32);
  bson_append_long(bb, "kernel64", stats->kernel64);
  bson_append_long(bb, "kernel32", stats->kernel32);
  bson_append_long(bb, "legacy16", stats->legacy16);
  bson_append_long(bb, "userlib", stats->userlib);
  bson_append_long(bb, "microcode", stats->microcode);
  bson_append_long(bb, "idle", stats->idle);
  bson_append_finish_object(bb);
}

void add_bson_PerCoreEvents(PerCoreEvents *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  add_bson_EventsInMode(&stats->cycles_in_mode, bb, "cycles_in_mode");
  add_bson_EventsInMode(&stats->insns_in_mode, bb, "insns_in_mode");
  add_bson_EventsInMode(&stats->uops_in_mode, bb, "uops_in_mode");
  bson_append_finish_object(bb);
}

void add_bson_PTLsimStats(PTLsimStats *stats, bson_buffer *bb, const char* obj_name) {
  bson_buffer *obj = bson_append_start_object(bb, obj_name);

  bson_append_long(bb, "snapshot_uuid", stats->snapshot_uuid);
  bson_append_string(bb, "snapshot_name", stats->snapshot_name);
  {
    const char *node_name = "summary";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    bson_append_long(bb, "cycles", stats->summary.cycles);
    bson_append_long(bb, "insns", stats->summary.insns);
    bson_append_long(bb, "uops", stats->summary.uops);
    bson_append_long(bb, "basicblocks", stats->summary.basicblocks);
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "simulator";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "version";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_string(bb, "build_timestamp", stats->simulator.version.build_timestamp);
      bson_append_long(bb, "svn_revision", stats->simulator.version.svn_revision);
      bson_append_string(bb, "svn_timestamp", stats->simulator.version.svn_timestamp);
      bson_append_string(bb, "build_hostname", stats->simulator.version.build_hostname);
      bson_append_string(bb, "build_compiler", stats->simulator.version.build_compiler);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "run";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "timestamp", stats->simulator.run.timestamp);
      bson_append_string(bb, "hostname", stats->simulator.run.hostname);
      bson_append_string(bb, "kernel_version", stats->simulator.run.kernel_version);
      bson_append_string(bb, "hypervisor_version", stats->simulator.run.hypervisor_version);
      bson_append_long(bb, "native_cpuid", stats->simulator.run.native_cpuid);
      bson_append_long(bb, "native_hz", stats->simulator.run.native_hz);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "config";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_string(bb, "config", stats->simulator.config.config);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "performance";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      {
        const char *node_name = "rate";
        bson_buffer *obj = bson_append_start_object(bb, node_name);
        bson_append_long(bb, "cycles_per_sec", stats->simulator.performance.rate.cycles_per_sec);
        bson_append_long(bb, "issues_per_sec", stats->simulator.performance.rate.issues_per_sec);
        bson_append_long(bb, "user_commits_per_sec", stats->simulator.performance.rate.user_commits_per_sec);
        bson_append_finish_object(bb);
      }

      bson_append_finish_object(bb);
    }

    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "decoder";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      const char *node_name = "throughput";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "basic_blocks", stats->decoder.throughput.basic_blocks);
      bson_append_long(bb, "x86_insns", stats->decoder.throughput.x86_insns);
      bson_append_long(bb, "uops", stats->decoder.throughput.uops);
      bson_append_long(bb, "bytes", stats->decoder.throughput.bytes);
      bson_append_finish_object(bb);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "x86_decode_type");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < DECODE_TYPE_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, decode_type_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < DECODE_TYPE_COUNT; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->decoder.x86_decode_type[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      const char *node_name = "bb_decode_type";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "all_insns_fast", stats->decoder.bb_decode_type.all_insns_fast);
      bson_append_long(bb, "some_complex_insns", stats->decoder.bb_decode_type.some_complex_insns);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "page_crossings";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      add_bson_obj_attr_bool(obj, "summable", 1);
      bson_append_long(bb, "within_page", stats->decoder.page_crossings.within_page);
      bson_append_long(bb, "crosses_page", stats->decoder.page_crossings.crosses_page);
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "bbcache";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "count", stats->decoder.bbcache.count);
      bson_append_long(bb, "inserts", stats->decoder.bbcache.inserts);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "invalidates");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < INVALIDATE_REASON_COUNT; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, invalidate_reason_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < INVALIDATE_REASON_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->decoder.bbcache.invalidates[i]);
        }
        bson_append_finish_object(arr);
      }
      bson_append_finish_object(bb);
    }

    {
      const char *node_name = "pagecache";
      bson_buffer *obj = bson_append_start_object(bb, node_name);
      bson_append_long(bb, "count", stats->decoder.pagecache.count);
      bson_append_long(bb, "inserts", stats->decoder.pagecache.inserts);
      {
        char numstr[8];
        bson_buffer *arr = bson_append_start_array(bb, "invalidates");
        {
          bson_buffer *label_arr = bson_append_start_array(bb, "label");
          for(int i = 0; i < INVALIDATE_REASON_COUNT; i++) {
            bson_numstr(numstr, i);
            bson_append_string(label_arr, numstr, invalidate_reason_names[i]);
          }
          bson_append_finish_object(arr);
        }

        for(int i = 0; i < INVALIDATE_REASON_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_long(arr, numstr, stats->decoder.pagecache.invalidates[i]);
        }
        bson_append_finish_object(arr);
      }
      bson_append_finish_object(bb);
    }

    bson_append_long(bb, "reclaim_rounds", stats->decoder.reclaim_rounds);
    bson_append_finish_object(bb);
  }

  add_bson_OutOfOrderCoreStats(&stats->ooocore_total, bb, "ooocore_total");
  add_bson_PerContextOutOfOrderCoreStats(&stats->ooocore_context_total, bb, "ooocore_context_total");
  bson_append_long(bb, "elapse_seconds", stats->elapse_seconds);
  {
    const char *node_name = "ooocore";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_OutOfOrderCoreStats(&stats->ooocore.total, bb, "total");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c0, bb, "c0");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c1, bb, "c1");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c2, bb, "c2");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c3, bb, "c3");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c4, bb, "c4");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c5, bb, "c5");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c6, bb, "c6");
    add_bson_OutOfOrderCoreStats(&stats->ooocore.c7, bb, "c7");
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "dcache";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_DataCacheStats(&stats->dcache.total, bb, "total");
    add_bson_DataCacheStats(&stats->dcache.c0, bb, "c0");
    add_bson_DataCacheStats(&stats->dcache.c1, bb, "c1");
    add_bson_DataCacheStats(&stats->dcache.c2, bb, "c2");
    add_bson_DataCacheStats(&stats->dcache.c3, bb, "c3");
    add_bson_DataCacheStats(&stats->dcache.c4, bb, "c4");
    add_bson_DataCacheStats(&stats->dcache.c5, bb, "c5");
    add_bson_DataCacheStats(&stats->dcache.c6, bb, "c6");
    add_bson_DataCacheStats(&stats->dcache.c7, bb, "c7");
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "external";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "assists");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < ASSIST_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, assist_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < ASSIST_COUNT; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->external.assists[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "l_assists");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < L_ASSIST_COUNT; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, light_assist_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < L_ASSIST_COUNT; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->external.l_assists[i]);
      }
      bson_append_finish_object(arr);
    }
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "traps");
      {
        bson_buffer *label_arr = bson_append_start_array(bb, "label");
        for(int i = 0; i < 256; i++) {
          bson_numstr(numstr, i);
          bson_append_string(label_arr, numstr, x86_exception_names[i]);
        }
        bson_append_finish_object(arr);
      }

      for(int i = 0; i < 256; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->external.traps[i]);
      }
      bson_append_finish_object(arr);
    }
    add_bson_PerCoreEvents(&stats->external.total, bb, "total");
    add_bson_PerCoreEvents(&stats->external.c0, bb, "c0");
    add_bson_PerCoreEvents(&stats->external.c1, bb, "c1");
    add_bson_PerCoreEvents(&stats->external.c2, bb, "c2");
    add_bson_PerCoreEvents(&stats->external.c3, bb, "c3");
    add_bson_PerCoreEvents(&stats->external.c4, bb, "c4");
    add_bson_PerCoreEvents(&stats->external.c5, bb, "c5");
    add_bson_PerCoreEvents(&stats->external.c6, bb, "c6");
    add_bson_PerCoreEvents(&stats->external.c7, bb, "c7");
    bson_append_finish_object(bb);
  }

  {
    const char *node_name = "memory";
    bson_buffer *obj = bson_append_start_object(bb, node_name);
    add_bson_PerCoreCacheStats(&stats->memory.total, bb, "total");
    add_bson_PerCoreCacheStats(&stats->memory.c0, bb, "c0");
    add_bson_PerCoreCacheStats(&stats->memory.c1, bb, "c1");
    add_bson_PerCoreCacheStats(&stats->memory.c2, bb, "c2");
    add_bson_PerCoreCacheStats(&stats->memory.c3, bb, "c3");
    add_bson_PerCoreCacheStats(&stats->memory.c4, bb, "c4");
    add_bson_PerCoreCacheStats(&stats->memory.c5, bb, "c5");
    add_bson_PerCoreCacheStats(&stats->memory.c6, bb, "c6");
    add_bson_PerCoreCacheStats(&stats->memory.c7, bb, "c7");
    add_bson_BusStats(&stats->memory.bus, bb, "bus");
    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "dcache_latency");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < 200; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->memory.dcache_latency[i]);
      }
      bson_append_finish_object(arr);
    }

    {
      char numstr[8];
      bson_buffer *arr = bson_append_start_array(bb, "icache_latency");
      bson_append_bool(bb, "histogram", (bson_bool_t)1);
      for(int i = 0; i < 200; i++) {
        bson_numstr(numstr, i);
        bson_append_long(arr, numstr, stats->memory.icache_latency[i]);
      }
      bson_append_finish_object(arr);
    }

    bson_append_finish_object(bb);
  }

  bson_append_finish_object(bb);
}

