// go into vector<Items*>, it->second = vector<Items*>
    for(auto it : items){
        // go into Items*, *jt = Items*
        for(auto jt : it.second){
            // go into Items.items(vector<Item*>), *kt = Item*
            for(auto kt : jt->items){
                if(kt->isContained){
                    continue;
                }
                Items* closure = new Items(closure_num++);
                closure->append(kt);
                kt->isContained = true;

                tmp.clear();
                tmp.insert(kt);
                // A->a.Bb, append B->.X into this closure
                while(!tmp.empty()){
                    auto lt = *tmp.begin();

                    // if B is non-terminal
                    if(lt->dot < lt->length && !lt->grammer->tail[lt->dot]->is_terminal){
                        // get all the items whose lhs with B
                        for(auto rt : items[lt->grammer->tail[lt->dot]->symbol]){
                            // append B->.x
                            if(!closure->append(*rt->items.begin())){
                                (*rt->items.begin())->isContained = true;
                                tmp.insert(*rt->items.begin());
                                tmp.erase(lt);
                            }
                        }
                    }
                    else{
                        tmp.erase(lt);
                    }
                }

                closures.push_back(closure);
            }
        }
    }



    // check if a given sym can produce epsilon recursively
bool Grammers::_produceE(char sym){
    if(sym == 'e'){
        return true;
    }

    bool a[20] = {false};

    for(int i = 0;i < length;i++){
        if(grammers[i]->head->symbol == sym){
            a[i] = true;
            for(auto j = grammers[i]->tail.cbegin();j != grammers[i]->tail.cend();j++){
                a[i] = a[i] && _produceE((*j)->symbol);
                // early stopping if there is a symbol that cannot derive epsilon
                if(!a[i]){
                    break;
                }
            }
        }
    }

    for(int i = 0;i < length;i++){
        if(a[i] == true){
            return true;
        }
    }
    return false;
}

// find out symbols that can produce epsilon
void Grammers::get_produce_e(Symbols &syms){
    for(auto i=syms.symbols.cbegin();i !=syms.symbols.cend();i++){
        (*i)->produce_e = _produceE((*i)->symbol);
    }
}