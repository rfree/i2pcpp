 
function make_amount(val) 
{
    if ( val > 1024 ) {
        val = val / 1024;
        if ( val > 1024) {
            return Math.floor(val) + "Mbps";
        }
        return Math.floor(val) + "Kbps";
    }
    return Math.floor(val) + "bps";
}

function calc_mean(arr) 
{
    var s = 0;
    for ( var ind = 0; ind < arr.length; ind++ ) {
        s += arr[ind];                             
    }
    return ( 0.0 + s ) / arr.length;
}

var new_i2np = function () {
    return { DBL: 0,
             DS: 0,
             DBS: 0,
             DBSR: 0,
             VTB: 0,
             VTBR: 0,
             TD: 0,
             TG: 0,
             G: 0,
             X: 0 };
}

var stats_new = function() 
{
    
    return {
        send : {
            hist: [],
            mean: 0,
            value: 0,
        },
        recv : {
            hist: [],
            mean: 0,
            value: 0,
        },
        hist_count: 0,
        hist_len: 0,
        hist_mean_interval: 5,
        tunnel : { //TODO: add more
            participating: 0
        },
        i2np_ib : new_i2np() ,
        i2np_ob : new_i2np() 
    };
}

var stats_update = function(stats, j_obj)
{
    var bw = j_obj.bandwidth;
    stats.send.value = bw[0] * 8;
    stats.recv.value = bw[1] * 8;
    stats.send.hist.push(stats.send.value);
    stats.recv.hist.push(stats.recv.value);
    stats.hist_len ++;
    stats.hist_count ++;
    while ( stats.hist_len > stats.hist_mean_interval ) {
        stats.send.hist.shift();
        stats.recv.hist.shift();
        stats.hist_len --;
    }
    stats.send.mean = calc_mean(stats.send.hist);
    stats.recv.mean = calc_mean(stats.recv.hist);

    stats.tunnel.participating = j_obj.tunnels.participating;
    
    stats.i2np_ib = new_i2np();
    stats.i2np_ob = new_i2np();
    
    var i2np_ib = j_obj.i2np[0];
    var i2np_ob = j_obj.i2np[1];

    for ( var k in i2np_ib ) {
        stats.i2np_ib[k] = i2np_ib[k];
    }

    for ( var k in i2np_ob ) {
        stats.i2np_ob[k] = i2np_ob[k];
    }

}

var stats_put_graph = function(stat, graph)
{
    graph.series.addData( { amount: stat.value , mean: stat.mean } );
}


var statsConnection = new WebSocket("ws://127.0.0.1:10010/stats");

statsConnection.onopen = function() { $("#connection_label").html("Connected"); }
statsConnection.onclose = function() { $("#connection_label").html("Disconnected"); }

function make_i2np_graph(elem, h) 
{
    var palette = new Rickshaw.Color.Palette( { scheme: 'munin' } );

    var graph = new Rickshaw.Graph({ 
        element: elem,
        renderer: 'bar',
        stroke: true,        
        //width: 500,
        series: new Rickshaw.Series.FixedDuration([
            { name:  'DS' , color: '#fd0' },
            { name:  'DBS' , color : '#df0' },
            { name:  'DBL' , color : '#0df' },
            { name:  'DBSR', color : '#0fd' },
            { name:  'VTB' , color : 'green' },
            { name:  'VTBR' , color : 'blue' },
            { name:  'TD' , color : 'orange' },
            { name:  'TG' , color : 'yellow' },
            { name:  'G' , color : '#01f' },
            { name:  'X' , color: 'red' }

        ],
        palette,
        {
        timeInterval: 1000,
        maxDataPoints: 50,
        timeBase: new Date().getTime() / 1000
        })
    });

    var hover = new Rickshaw.Graph.HoverDetail( {
        graph: graph,
        formatter: function(series, x, y) {
            return series.name + " : " + y;
        }
    });

    var x_axis = new Rickshaw.Graph.Axis.Time({
        graph: graph,
        ticksTreatment: 'glow',
        timeFixture: new Rickshaw.Fixtures.Time.Local()
    })
    var y_axis = new Rickshaw.Graph.Axis.Y({
        graph: graph,
        tickFormat: Rickshaw.Fixtures.Number.formatKMBT,
        ticksTreatment: 'glow'
    });
    var render = function() { x_axis.render(); y_axis.render(); };
    return [graph, render]
}

function make_graph(elem, h, not_bw) 
{
    var palette = new Rickshaw.Color.Palette( { scheme: 'spectrum14' } );

    var graph = new Rickshaw.Graph({ 
        element: elem,
        renderer: 'multi',
        //width: 500,
        stroke: true,
        series: new Rickshaw.Series.FixedDuration([
            { name: 'mean'  , renderer: 'bar' },
            { name: 'amount' , renderer: 'line' , color: '#33d' }, 
            ],
            palette,
            {
                timeInterval: 1000,
                maxDataPoints: 50,
                timeBase: new Date().getTime() / 1000
            })
    });

    var hover = new Rickshaw.Graph.HoverDetail( {
        graph: graph,
        formatter: function(series, x, y) {
            if (not_bw) {
                return y;
            } else {
                return make_amount(y);
            }
        }
    });

    var x_axis = new Rickshaw.Graph.Axis.Time({
        graph: graph,
        ticksTreatment: 'glow',
        timeFixture: new Rickshaw.Fixtures.Time.Local()
    })
    var y_axis = new Rickshaw.Graph.Axis.Y({
        graph: graph,
        tickFormat: Rickshaw.Fixtures.Number.formatKMBT,
        ticksTreatment: 'glow'
    });
    var render = function() { x_axis.render(); y_axis.render(); };
    return [graph, render]
}

var part = make_graph($("#graph_participating").get(0), 0, true);

var render_part = part[1];
var graph_part = part[0];

render_part();
graph_part.render();

var i2np = make_i2np_graph($("#graph_i2np_ib").get(0), 0);
var render_i2np_ib = i2np[1];
var graph_i2np_ib = i2np[0];

render_i2np_ib();
graph_i2np_ib.render();

var i2np = make_i2np_graph($("#graph_i2np_ob").get(0), 0);
var render_i2np_ob = i2np[1];
var graph_i2np_ob = i2np[0];

render_i2np_ob();
graph_i2np_ob.render();

var send = make_graph($("#graph_send").get(0), 0)
var send_axis_render = send[1];
var graph_send = send[0];

send_axis_render();
graph_send.render();

var recv = make_graph($("#graph_recv").get(0), 0);
var recv_axis_render = recv[1];
var graph_recv = recv[0];

recv_axis_render();
graph_recv.render();

var peers = make_graph($("#graph_peers").get(0), 0, true);
var peers_axis_render = peers[1];
var graph_peers = peers[0];

recv_axis_render();
graph_recv.render();


var update_label = function(stat, elem) 
{
    $(elem).html("Current: " + make_amount(stat.value) + " Mean: " + make_amount(stat.mean));
}

var stats = stats_new();

statsConnection.onmessage = function(msg) 
{
    var data = JSON.parse(msg.data);
    stats_update(stats, data);
    stats_put_graph(stats.recv, graph_recv);
    graph_recv.render();

    stats_put_graph(stats.send, graph_send);
    graph_send.render(); 

    graph_part.series.addData({amount: stats.tunnel.participating, mean: 0});
    graph_part.render();

    graph_peers.series.addData({ amount: data.peers, mean: 0});
    graph_peers.render();

    update_label(stats.recv, "#recv_str");
    update_label(stats.send, "#send_str");

    $("#peers_str").html(data.peers);
    $("#participating_str").html(stats.tunnel.participating);

    graph_i2np_ib.series.addData(stats.i2np_ib);
    graph_i2np_ib.render();    

    graph_i2np_ob.series.addData(stats.i2np_ob);
    graph_i2np_ob.render();
};
