///////////////////////////////////////////////////////////////////////////

var st, en;
var i;

///////////////////////////////////////////////////////////////////////////
print("\n** Entering long loop...\n");
///////////////////////////////////////////////////////////////////////////

st = new Date();

for(i = 0; i < 750000; i++)
{
  /* Do nothing */
}

en = new Date();

print("That took " + (en.getTime() - st.getTime()) / 1000 + " seconds");

///////////////////////////////////////////////////////////////////////////
print("\n** Loop with calls...\n");
///////////////////////////////////////////////////////////////////////////

function square(x)
{
  return(x*x);
}

st = new Date();

for(i = 0; i < 100000; i++)
{
  var y = square(20);
  var z = square(10);

  y = square(z);
  z = y / z;
}

en = new Date();

print("That took " + (en.getTime() - st.getTime()) / 1000 + " seconds");

///////////////////////////////////////////////////////////////////////////
print("\n** Loop with allocations...\n");
///////////////////////////////////////////////////////////////////////////

st = new Date();

for(i = 0; i < 50000; i++)
{
  var y = new Date();
  var o = new Object;
  var z = Math.PI;
  var a = i * 1.2;
}

en = new Date();

print("That took " + (en.getTime() - st.getTime()) / 1000 + " seconds");

///////////////////////////////////////////////////////////////////////////
print("\n** Building big string...\n");
///////////////////////////////////////////////////////////////////////////

st = new Date();

{
  var s = "";

  for(i = 0; i < 2000; i++)
  {
    s += i + " ";
  }
}

en = new Date();

print("That took " + (en.getTime() - st.getTime()) / 1000 + " seconds");

///////////////////////////////////////////////////////////////////////////
print("\n** Building huge string...\n");
///////////////////////////////////////////////////////////////////////////

st = new Date();

{
  var s = "";

  for(i = 0; i < 4000; i++)
  {
    s += i + " ";
  }
}

en = new Date();

print("That took " + (en.getTime() - st.getTime()) / 1000 + " seconds");
