truncate test_insert;
truncate test_update;
insert into test_update select mod(i,100), -1 from generate_series(0,10000) s(i);
vacuum analyze;

