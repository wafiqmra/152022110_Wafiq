<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Data Cuaca IoT</title>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="styles.css">
</head>
<body>
    <div class="container">
        <header>
            <h1>Data Cuaca IoT</h1>
        </header>
        
        <div id="summary" class="box">
            <h2>Ringkasan Data</h2>
            <p><strong>Suhu Maksimum:</strong> <span id="suhumax"></span>°C</p>
            <p><strong>Suhu Minimum:</strong> <span id="suhumin"></span>°C</p>
            <p><strong>Suhu Rata-rata:</strong> <span id="suhurata"></span>°C</p>
        </div>

        <div id="temperature-humidity" class="box">
            <h3>Data Tertinggi Suhu dan Kelembapan</h3>
            <table id="data-table">
                <thead>
                    <tr>
                        <th>Index</th>
                        <th>Suhu</th>
                        <th>Kelembapan</th>
                        <th>Kecerahan</th>
                        <th>Waktu</th>
                    </tr>
                </thead>
                <tbody>
                </tbody>
            </table>
        </div>
    </div>

    <script src="script.js"></script>
</body>
</html>
