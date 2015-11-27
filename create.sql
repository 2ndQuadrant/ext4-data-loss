create table test_insert (id bigint, val text);
create table test_update as select * from test_insert where false;
insert into test_update select mod(i,100), -1 from generate_series(0,10000) s(i);
create index on test_update (id);
