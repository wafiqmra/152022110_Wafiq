<?php
// Pengaturan koneksi ke database
$host = 'localhost';
$username = 'root';
$password = '';
$database = 'iot';

// Membuat koneksi
$conn = new mysqli($host, $username, $password, $database);

// Periksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}

// Query untuk mengambil data dari tabel
$sql = "SELECT * FROM tb_cuaca"; // Ganti dengan nama tabel Anda
$result = $conn->query($sql);

// Inisialisasi array untuk menyimpan data suhu
$data = array();
$suhu_values = array();
$humidity_values = array();
$kecerahan_values = array();
$tanggal_values = array();

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $data[] = $row;
        $suhu_values[] = $row['suhu'];
        $humidity_values[] = $row['humid'];
        $kecerahan_values[] = $row['lux'];
        $tanggal_values[] = $row['ts'];
    }
    
    // Menghitung suhu maksimum, minimum, dan rata-rata
    $max_suhu = max($suhu_values);
    $min_suhu = min($suhu_values);
    $average_suhu = array_sum($suhu_values) / count($suhu_values);
    
    // Mendapatkan nilai maksimum untuk suhu dan kelembapan sekaligus
    $nilai_suhu_max_humid_max = array();
    foreach ($data as $key => $value) {
        if ($value['suhu'] == $max_suhu && $value['humid'] == max($humidity_values)) {
            $nilai_suhu_max_humid_max[] = array(
                'idx' => $key,
                'suhu' => $value['suhu'],
                'humid' => $value['humid'],
                'kecerahan' => $value['lux'],
                'timestamp' => $value['ts']
            );
        }
    }
    
    // Mendapatkan bulan dan tahun dari suhu maksimum
    $month_year_max = array();
    foreach ($nilai_suhu_max_humid_max as $entry) {
        $month_year = date("n-Y", strtotime($entry['timestamp']));
        $month_year_max[] = array('month_year' => $month_year);
    }

    // Menyusun data summary di bagian atas
    $data_summary = array(
        'suhumax' => $max_suhu,
        'suhumin' => $min_suhu,
        'suhurata' => $average_suhu,
        'nilai_suhu_max_humid_max' => $nilai_suhu_max_humid_max,
        'month_year_max' => $month_year_max
    );
} else {
    $data_summary = null;
}

// Mengatur header untuk JSON
header('Content-Type: application/json');

// Menampilkan data dalam format JSON
$response = $data_summary;
echo json_encode($response, JSON_PRETTY_PRINT);

$conn->close();
?>
