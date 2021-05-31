```flow
st=>start: Start
op1=>operation: Read in one word or one symbol as token
sub1=>subroutine: My Subroutine
cond=>condition: Is the token a keyword or a legal signal?
cond2=>condition: Is the token legal?
io1=>inputoutput: Output corresponding triple
io2=>inputoutput: Append it into the IDENTIFIERS/CONSTS
io3=>inputoutput: Skip the entire token and output error
para=>parallel: parallel tasks
end=>end

st->op1->cond
cond(yes)->io1->end
cond(no)->cond2
cond2(yes)->io2->end
cond2(no)->io3->end
```