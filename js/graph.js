function initGraph(graphData) {

    const canvas = document.getElementById("graph");
    const ctx = canvas.getContext("2d");

    let width, height;

    function resize() {
        width = canvas.width = window.innerWidth;
        height = canvas.height = window.innerHeight;
    }
    window.addEventListener("resize", resize);
    resize();

    let mouseX = 0;
    let mouseY = 0;

    let offsetX = 0;
    let offsetY = 0;

    canvas.addEventListener("mousemove", e => {
        mouseX = e.clientX;
        mouseY = e.clientY;
    });

    const nodes = graphData.nodes.map(n => ({
        id: n.id,
        label: n.label,
        x: 0.3 + Math.random() * width * 0.7,
        y: 0.3 + Math.random() * height * 0.7,
        vx: 0,
        vy: 0
    }));

    const nodeMap = {};
    nodes.forEach(n => nodeMap[n.id] = n);

    const edges = graphData.connections.map(c => ({
        a: nodeMap[c[0]],
        b: nodeMap[c[1]],
        intensity: c[2] ?? 1
    }));

    const REPULSION = 4000;
    const SPRING = 0.01;
    const REST_LENGTH = 120;
    const DAMPING = 0.85;
    const MIN_DIST = 40;

    function physics() {
        for (let i = 0; i < nodes.length; i++) {
            for (let j = i + 1; j < nodes.length; j++) {

                let dx = nodes[j].x - nodes[i].x;
                let dy = nodes[j].y - nodes[i].y;
                let dist = Math.sqrt(dx*dx + dy*dy) + 0.01;
                let force = REPULSION / (dist * dist);
                let fx = force * dx / dist;
                let fy = force * dy / dist;

                nodes[i].vx -= fx;
                nodes[i].vy -= fy;
                nodes[j].vx += fx;
                nodes[j].vy += fy;

                if (dist < MIN_DIST) {
                    let push = (MIN_DIST - dist) * 0.5;
                    nodes[i].x -= dx/dist * push;
                    nodes[i].y -= dy/dist * push;
                    nodes[j].x += dx/dist * push;
                    nodes[j].y += dy/dist * push;
                }
            }
        }

        edges.forEach(e => {
            let dx = e.b.x - e.a.x;
            let dy = e.b.y - e.a.y;
            let dist = Math.sqrt(dx*dx + dy*dy) + 0.01;

            const springStrength = SPRING * e.intensity;
            const rest = REST_LENGTH / Math.sqrt(e.intensity);
            let force = springStrength * (dist - rest);
            let fx = force * dx/dist;
            let fy = force * dy/dist;

            e.a.vx += fx;
            e.a.vy += fy;
            e.b.vx -= fx;
            e.b.vy -= fy;
        });

        nodes.forEach(n => {
            n.vx *= DAMPING;
            n.vy *= DAMPING;
            n.x += n.vx;
            n.y += n.vy;

            n.x = Math.max(20, Math.min(width - 20, n.x));
            n.y = Math.max(20, Math.min(height - 20, n.y));
        });
    }

    function updateOffset() {
        const targetX = (mouseX - width/2) * -0.15;
        const targetY = (mouseY - height/2) * -0.15;
        offsetX += (targetX - offsetX) * 0.1;
        offsetY += (targetY - offsetY) * 0.1;
    }

    function draw() {
        ctx.fillStyle = "black";
        ctx.fillRect(0,0,width,height);

        ctx.save();
        ctx.translate(offsetX, offsetY);

        edges.forEach(e => {
            const w = 0.6 + e.intensity * 4;

            // Culoarea gri în funcție de intensitate (0.3 -> slab, 1 -> intens)
            const t = (e.intensity);
            const colorVal = Math.floor(t * 255); // 50 -> 255
            const alpha = 0.3 + t * 0.7;

            ctx.lineWidth = w;
            ctx.strokeStyle = `rgba(${colorVal}, 0, 0, ${alpha})`;

            // Glow subtil
            ctx.shadowBlur = 6 * t;
            ctx.shadowColor = `rgba(255,0,0,${alpha*0.7})`;

            ctx.beginPath();
            ctx.moveTo(e.a.x, e.a.y);
            ctx.lineTo(e.b.x, e.b.y);
            ctx.stroke();

            ctx.shadowBlur = 0; // reset glow
        });

        nodes.forEach(n => {
            ctx.fillStyle = "black";
	    ctx.strokeStyle = "red";
	    ctx.shadowBlur = 2;
	    ctx.shadowColor = "rgba(255,0,0,0.4)";
	    ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.arc(n.x, n.y, 6, 0, Math.PI*2);
            ctx.fill();
	    ctx.stroke();

            ctx.fillStyle = "white";
            ctx.font = "12px sans-serif";
            ctx.fillText(n.label, n.x + 8, n.y - 8);
        });

        ctx.restore();
    }

    function loop() {
        physics();
        updateOffset();
        draw();
        requestAnimationFrame(loop);
    }

    loop();
}
