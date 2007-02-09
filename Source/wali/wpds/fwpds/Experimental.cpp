
void IntraGraph::compressGraphAggressive(list<int> &ts, vector<int> &cs,
                                         list<int> &nts, vector<int> &ncs,
                                         vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                                         map<int,int> &orig_to_compress) {
  int i,j,k,v,u,w;
  int m = cs.size();
  int n = nnodes;
  assert(n == (int)ts.size());

  set<int> cnodes_id;
  u = 0;
  // add source node
  cnodes.push_back(nodes[0]);
  orig_to_compress[0] = u++;
  cnodes_id.insert(0);
  // add call,ret nodes
  for(i=0;i<(int)updatable_edges.size(); i++) {
    int c = edges[updatable_edges[i]].src;
    int r = edges[updatable_edges[i]].tgt;
    if(orig_to_compress.find(c) == orig_to_compress.end()) {
      cnodes.push_back(nodes[c]);
      orig_to_compress[c] = u++;
      cnodes_id.insert(c);
    }
    if(orig_to_compress.find(r) == orig_to_compress.end()) {
      cnodes.push_back(nodes[r]);
      orig_to_compress[r] = u++;
      cnodes_id.insert(r);
    }
  }
  // add out nodes
  list<int>::iterator nit = out_nodes_intra->begin();
  for(; nit != out_nodes_intra->end(); nit++) {
    int on = *nit;
    if(orig_to_compress.find(on) == orig_to_compress.end()) {
      cnodes.push_back(nodes[on]);
      orig_to_compress[on] = u++;
      cnodes_id.insert(on);
    }
  }
  // add cutset nodes
  for(i=0;i<m;i++) {
    cnodes_id.insert(cs[i]);
  }

  map<int,int>::iterator cit;
  set<int>::iterator cn_it;

  sem_elem_t **temp = new sem_elem_t *[n];
  for(i=0;i<n;i++) {
    temp[i] = new sem_elem_t[n];
  }
  for(cn_it = cnodes_id.begin(); cn_it != cnodes_id.end(); cn_it++) {
    v = *cn_it;
    for(j=0;j<n;j++) {
      temp[v][j] = se->zero();
    }
  }

  // Solve for (_,_) where first node made it to cnodes_id
  // no node in cnodes_id have incoming edges
  for(cn_it = cnodes_id.begin(); cn_it != cnodes_id.end(); cn_it++) {
    v = *cn_it;
    list<int>::iterator beg = ts.begin();
    list<int>::iterator end = ts.end();
    while(*beg != v) {
      beg++;
    }
    temp[v][v] = se->one();
    beg++;
    while(beg != end) {
      u = *beg;
      if(cnodes_id.find(u) != cnodes_id.end()) {
        beg++;
        continue;
      }
      list<int>::iterator beg2 = nodes[u].incoming.begin();
      list<int>::iterator end2 = nodes[u].incoming.end();
      while(beg2 != end2) {
        w = edges[*beg2].src;
        if(edges[*beg2].updatable) {
          beg2++;
          continue;
        }
        temp[v][u] = temp[v][u]->combine(extend(temp[v][w],edges[*beg2].weight));
        STAT(stats.ncombine++);
        beg2++;
      }
      beg++;
    }
  }

  // Now add the incoming edges
  for(cn_it = cnodes_id.begin(); cn_it != cnodes_id.end(); cn_it++) {
    u = *cn_it;
    set<int>::iterator cn_it2;
    for(cn_it2 = cnodes_id.begin(); cn_it2 != cnodes_id.end(); cn_it2++) {
      v = *cn_it2;
      list<int>::iterator beg = nodes[v].incoming.begin();
      list<int>::iterator end = nodes[v].incoming.end();
      while(beg != end) {
        if(edges[*beg].updatable) {
          beg++;
          continue;
        }
        w = edges[*beg].src;
        if(cnodes_id.find(w) == cnodes_id.end()) {
          temp[u][v] = temp[u][v]->combine(extend(temp[u][w],edges[*beg].weight));
        } else {
          temp[w][v] = temp[w][v]->combine(edges[*beg].weight);
        }
        STAT(stats.ncombine++);
        beg++;
      }
    }
  }
  // Solve APSP on cutsets
  int ck,ci,cj;
  for(k=0;k<m;k++) {
    ck = cs[k];
    for(i=0;i<m;i++) {
      ci = cs[i];
      for(j=0;j<m;j++) {
        cj = cs[j];
        temp[ci][cj] = temp[ci][cj]->combine(extend(temp[ci][ck], extend(temp[ck][ck]->star(), temp[ck][cj])));
        STAT(stats.ncombine++);
        STAT(stats.nstar++);
      }
    }
  }
  // Solve (_,ci)
  for(cn_it = cnodes_id.begin(); cn_it != cnodes_id.end(); cn_it++) {
    u = *cn_it;
    for(i=0;i<m;i++) {
      ci = cs[i];
      for(j=0;j<m;j++) {
        cj = cs[j];
        temp[u][ci] = temp[u][ci]->combine(extend(temp[u][cj],temp[cj][ci]));
        STAT(stats.ncombine++);
      }
    }
  }

  // fill up cedges
  for(i=0;i<(int)cnodes.size();i++) {
    cnodes[i].outgoing.clear();
    cnodes[i].incoming.clear();
    cnodes[i].visited = 0;
    cnodes[i].iscutset = false;
  }

  for(i=0;i<(int)cnodes.size();i++) {
    u = nodeno(cnodes[i]);
    for(j=0;j<(int)cnodes.size();j++) {
      v = nodeno(cnodes[j]);
      IntraGraphEdge ed(i,j,temp[u][v],false);
      for(k=0;k<m;k++) {
        w = cs[k];
        ed.weight = ed.weight->combine(extend(temp[u][w],temp[w][v]));
        STAT(stats.ncombine++);
      }
      if(!ed.weight->equal(se->zero()) && !(i==j && ed.weight->equal(se->one()))) {
        //ed.weight->print(cout << i << "," << j << ":") << "\n";
        ed.regexp = RegExp::constant(ed.weight);
        cedges.push_back(ed);
        int e = cedges.size() - 1;
        cnodes[i].outgoing.push_back(e);
        cnodes[j].incoming.push_back(e);
      }
    }
  }
  // put in updatable edges
  for(i=0;i<(int)updatable_edges.size();i++) {
    IntraGraphEdge ed(edges[updatable_edges[i]]);
    ed.src = orig_to_compress[ed.src];
    ed.tgt = orig_to_compress[ed.tgt];
    cedges.push_back(ed);
    int e = cedges.size() - 1;
    cnodes[ed.src].outgoing.push_back(e);
    cnodes[ed.tgt].incoming.push_back(e);
  }
  // build new topsort and cutset lists
  topSort(cnodes,cnodes.size(),cedges,cedges.size(),nts,ncs,false,false);

  // delete stuff
    
  for(i = 0; i < n; i++) {
    delete [] temp[i];
  }
  delete [] temp;
}

void IntraGraph::compressGraph(list<int> &ts, vector<int> &cs,
                               list<int> &nts, vector<int> &ncs,
                               vector<IntraGraphNode> &cnodes, vector<IntraGraphEdge> &cedges,
                               map<int,int> &orig_to_compress) {
  int i,j,v,u,w;
  int m = cs.size();
  int n = nnodes;
  assert(n == (int)ts.size());

  u = 0;
  // add source node
  cnodes.push_back(nodes[0]);
  orig_to_compress[0] = u++;
  // add call,ret nodes
  for(i=0;i<(int)updatable_edges.size(); i++) {
    int c = edges[updatable_edges[i]].src;
    int r = edges[updatable_edges[i]].tgt;
    if(orig_to_compress.find(c) == orig_to_compress.end()) {
      cnodes.push_back(nodes[c]);
      orig_to_compress[c] = u++;
    }
    if(orig_to_compress.find(r) == orig_to_compress.end()) {
      cnodes.push_back(nodes[r]);
      orig_to_compress[r] = u++;
    }
  }
  // add out nodes
  list<int>::iterator nit = out_nodes_intra->begin();
  for(; nit != out_nodes_intra->end(); nit++) {
    int on = *nit;
    if(orig_to_compress.find(on) == orig_to_compress.end()) {
      cnodes.push_back(nodes[on]);
      orig_to_compress[on] = u++;
    }
  }
  // add cutset nodes
  for(i=0;i<m;i++) {
    if(orig_to_compress.find(cs[i]) == orig_to_compress.end()) {
      cnodes.push_back(nodes[cs[i]]);
      orig_to_compress[cs[i]] = u++;
    }
  }

  map<int,int>::iterator cit;

  sem_elem_t **temp = new sem_elem_t *[n];
  for(i=0;i<n;i++) {
    temp[i] = new sem_elem_t[n];
  }
  for(cit = orig_to_compress.begin(); cit != orig_to_compress.end(); cit++) {
    v = cit->first;
    for(j=0;j<n;j++) {
      temp[v][j] = se->zero();
    }
  }

  // Solve for (_,_) where first node made it to cnodes
  for(cit = orig_to_compress.begin(); cit != orig_to_compress.end(); cit++) {
    v = cit->first;
    list<int>::iterator beg = ts.begin();
    list<int>::iterator end = ts.end();
    while(*beg != v) {
      beg++;
    }
    temp[v][v] = se->one();
    beg++;
    while(beg != end) {
      u = *beg;
      if(nodes[u].iscutset) {
        beg++;
        continue;
      }
      list<int>::iterator beg2 = nodes[u].incoming.begin();
      list<int>::iterator end2 = nodes[u].incoming.end();
      while(beg2 != end2) {
        w = edges[*beg2].src;
        if(edges[*beg2].updatable) {
          beg2++;
          continue;
        }
        temp[v][u] = temp[v][u]->combine(extend(temp[v][w],edges[*beg2].weight));
        STAT(stats.ncombine++);
        beg2++;
      }
      beg++;
    }
  }

  // Add cutset incoming edges
  for(cit = orig_to_compress.begin(); cit != orig_to_compress.end(); cit++) {
    u = cit->first;
    for(i=0;i<m;i++) {
      v = cs[i];
      list<int>::iterator beg = nodes[v].incoming.begin();
      list<int>::iterator end = nodes[v].incoming.end();
      while(beg != end) {
        if(edges[*beg].updatable) {
          beg++;
          continue;
        }
        w = edges[*beg].src;
        if(orig_to_compress.find(w) == orig_to_compress.end()) {
          temp[u][v] = temp[u][v]->combine(extend(temp[u][w],edges[*beg].weight));
        } else {
          temp[w][v] = temp[w][v]->combine(edges[*beg].weight);
        }
        beg++;
      }
    }
  }

  // fill up cedges
  for(i=0;i<(int)cnodes.size();i++) {
    cnodes[i].outgoing.clear();
    cnodes[i].incoming.clear();
    cnodes[i].visited = 0;
    cnodes[i].iscutset = false;
  }

  for(i=0;i<(int)cnodes.size();i++) {
    u = nodeno(cnodes[i]);
    for(j=0;j<(int)cnodes.size();j++) {
      v = nodeno(cnodes[j]);
      IntraGraphEdge ed(i,j,temp[u][v],false);
      if(!ed.weight->equal(se->zero()) && !(i==j && ed.weight->equal(se->one()))) {
        //ed.weight->print(cout << i << "," << j << ":") << "\n";
        ed.regexp = RegExp::constant(ed.weight);
        cedges.push_back(ed);
        int e = cedges.size() - 1;
        cnodes[i].outgoing.push_back(e);
        cnodes[j].incoming.push_back(e);
      }
    }
  }
  // put in updatable edges
  for(i=0;i<(int)updatable_edges.size();i++) {
    IntraGraphEdge ed(edges[updatable_edges[i]]);
    ed.src = orig_to_compress[ed.src];
    ed.tgt = orig_to_compress[ed.tgt];
    cedges.push_back(ed);
    int e = cedges.size() - 1;
    cnodes[ed.src].outgoing.push_back(e);
    cnodes[ed.tgt].incoming.push_back(e);
  }
  // build new topsort and cutset lists
  for(i=0;i<m;i++) {
    ncs.push_back(orig_to_compress[cs[i]]);
  }
  list<int>::iterator tsit;
  for(tsit = ts.begin(); tsit != ts.end(); tsit++) {
    if(orig_to_compress.find(*tsit) != orig_to_compress.end())
      nts.push_back(orig_to_compress[*tsit]);
  }

  // delete stuff
    
  for(i = 0; i < n; i++) {
    delete [] temp[i];
  }
  delete [] temp;
}

// O(n^3) method
void IntraGraph::basicRegExp(bool compress_regexp) {
  int n = (int)nnodes;
  int m = (int)nedges;
  int i,j,u,v,w;
  reg_exp_t **reg = new reg_exp_t* [n];
  for(i = 0; i < n; i++) {
    reg[i] = new reg_exp_t[n];
    for(j = 0; j < n; j++) {
      reg[i][j] = RegExp::constant(se->zero());
    }
  }
  for(i = 0; i < m; i++) {
    if(edges[i].updatable) {
      reg[edges[i].src][edges[i].tgt] = RegExp::combine(reg[edges[i].src][edges[i].tgt],
                                                        RegExp::updatable(edges[i].updatable_no, edges[i].weight));
    } else {
      reg[edges[i].src][edges[i].tgt] = RegExp::combine(reg[edges[i].src][edges[i].tgt],
                                                        RegExp::constant(edges[i].weight));
    }
  }
    
  // construct the path sequence
  for(v = 0; v < n; v++) {
    reg[v][v] = RegExp::star(reg[v][v]);
    for(u = v+1; u < n; u++) {
      if(reg[u][v]->isZero()) continue;
      reg[u][v] = RegExp::extend(reg[u][v],reg[v][v]);
      for(w = v+1; w < n; w++) {
        if(reg[v][w]->isZero()) continue;
        reg[u][w] = RegExp::combine(reg[u][w],RegExp::extend(reg[u][v],reg[v][w]));
      }
    }
  }

  // initialize answer
  for(i = 1; i < n; i++) {
    nodes[i].regexp = RegExp::constant(se->zero());
  }
  nodes[0].regexp = RegExp::constant(se->one());
  // now traverse the path sequence in order
  for(u = 0; u < n; u++) {
    for(w = u; w < n; w++) {
      if(w == u && reg[u][w]->isOne()) continue;
      if(reg[u][w]->isZero()) continue;
      if(u == w) {
        nodes[u].regexp = RegExp::extend(nodes[u].regexp,reg[u][w]);
      } else {
        nodes[w].regexp = RegExp::combine(nodes[w].regexp,RegExp::extend(nodes[u].regexp,reg[u][w]));
      }
    }
  }
  for(u = n-1; u >= 0; u--) {
    for(w = u-1; w >= 0; w--) {
      if(reg[u][w]->isZero()) continue;
      if(u == w) {
        nodes[u].regexp = RegExp::extend(nodes[u].regexp,reg[u][w]);
      } else {
        nodes[w].regexp = RegExp::combine(nodes[w].regexp,RegExp::extend(nodes[u].regexp,reg[u][w]));
      }
    }
  }
  if(compress_regexp) {
    reg_exp_cache_t cache;
    for(i = 0; i < n; i++) {
      FWPDSDBGS(nodes[i].regexp->print(cout) << "\n");
      nodes[i].regexp = RegExp::compress(nodes[i].regexp,cache);
      //nodes[i].regexp->get_weight();
      FWPDSDBGS(nodes[i].regexp->print(cout) << "\n");
    }
    cache.clear();
  }
    
  for(i = 0; i < n; i++) {
    delete [] reg[i];
  }
  delete [] reg;
}
